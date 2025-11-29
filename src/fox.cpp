// Copyright (c) 2025 and onwards The McFoxxIM Authors.
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "fox.h"

#include <fcitx-utils/log.h>
#include <fcitx/addonmanager.h>
#include <fcitx/candidatelist.h>
#include <fcitx/event.h>
#include <fcitx/inputcontext.h>
#include <fcitx/inputpanel.h>
#include <fcitx/instance.h>
#include <fcitx/text.h>

namespace McFoxIM {

class FoxCandidate : public fcitx::CandidateWord {
 public:
  FoxCandidate(FoxEngine* engine, std::string text, int index)
      : fcitx::CandidateWord(fcitx::Text(text)),
        engine_(engine),
        index_(index) {}

  void select(fcitx::InputContext* context) const override {
    engine_->selectCandidate(index_, context);
  }

 private:
  FoxEngine* engine_;
  int index_;
};


#if USE_LEGACY_FCITX5_API_STANDARDPATH
// Note: The locate() method provided by fcitx::StandardPath::global() only
// supports files but not directories. So we use scanDirectories() instead.
std::string findFoxDataPath() {
  std::string foundPath = "";
  std::string targetSubPath = "fox/data";

  fcitx::StandardPath::global().scanDirectories(
      fcitx::StandardPath::Type::PkgData,
      [&](const std::string& basePath, bool /*Unused*/) -> bool {
        std::filesystem::path p =
            std::filesystem::path(basePath) / targetSubPath;
        if (std::filesystem::exists(p) && std::filesystem::is_directory(p)) {
          foundPath = p.string();
          return false;
        }
        return true;
      });

  return foundPath;
}
#else
std::string findFoxDataPath() {
  std::string targetSubPath = "fox/data";
  return fcitx::StandardPaths::global().locateDir(
      fcitx::StandardPaths::Type::PkgData, targetSubPath);
}
#endif

FoxEngine::FoxEngine(fcitx::Instance* /* Unused */)
    : fcitx::InputMethodEngineV2() {
  std::string dataPath = findFoxDataPath();
  if (dataPath.empty()) {
    FCITX_ERROR() << "FoxEngine data path is empty. Cannot initialize input "
                     "table manager.";
    throw std::runtime_error("FoxEngine data path is empty.");
  }
  tableManager_ = std::make_unique<InputTableManager>(dataPath);
  // Set default table if available
  if (!tableManager_->availableTables().empty()) {
    tableManager_->setTable(0);
  }

  completer_ = std::make_unique<Completer>(
      [this]() -> const InputTable& { return tableManager_->currentTable(); });

  keyHandler_ = std::make_unique<KeyHandler>(*completer_);
  state_ = std::make_unique<InputState::EmptyState>();

  reloadConfig();
}

void FoxEngine::activate(const fcitx::InputMethodEntry& entry,
                         fcitx::InputContextEvent& /* Unused */) {
  std::string tableName = entry.uniqueName();
  if (tableName.rfind("fox_", 0) == 0) {
    tableName = tableName.substr(4);
  }

  if (currentTableName_ != tableName) {
    tableManager_->setTable(tableName);
    currentTableName_ = tableName;
  }
}

void FoxEngine::reset(const fcitx::InputMethodEntry& entry,
                      fcitx::InputContext& context) {
  FCITX_UNUSED(entry);
  if (auto inputState =
          dynamic_cast<InputState::InputtingState*>(state_.get())) {
    if (!inputState->composingBuffer().empty()) {
      context.commitString(inputState->composingBuffer());
    }
  }
  enterState(std::make_unique<InputState::EmptyState>(), &context);
}

void FoxEngine::keyEvent(const fcitx::InputMethodEntry& entry,
                         fcitx::KeyEvent& keyEvent) {
  FCITX_UNUSED(entry);
  if (keyEvent.isRelease()) {
    return;
  }

  auto context = keyEvent.inputContext();

  bool handled = keyHandler_->handle(
      keyEvent, *state_,
      [this, context](std::unique_ptr<InputState::InputState> newState) {
        enterState(std::move(newState), context);
      },
      []() {
        // On error, maybe beep?
      });

  if (handled) {
    keyEvent.accept();
  } else {
    if (auto inputState =
            dynamic_cast<InputState::InputtingState*>(state_.get())) {
      if (!inputState->composingBuffer().empty()) {
        context->commitString(inputState->composingBuffer());
        enterState(std::make_unique<InputState::EmptyState>(), context);
      }
    }
  }
}

void FoxEngine::selectCandidate(int index, fcitx::InputContext* context) {
  if (auto inputState =
          dynamic_cast<InputState::InputtingState*>(state_.get())) {
    const auto& candidates = inputState->candidatesInCurrentPage();
    if (index >= 0 && index < static_cast<int>(candidates.size())) {
      const auto& selectedCandidate = candidates[index];
      enterState(std::make_unique<InputState::CommittingState>(
                     selectedCandidate.displayText() + " "),
                 context);
    }
  }
}

void FoxEngine::enterState(std::unique_ptr<InputState::InputState> newState,
                           fcitx::InputContext* context) {
  if (dynamic_cast<InputState::EmptyState*>(newState.get())) {
    handleEmptyState(context);
    state_ = std::move(newState);
  } else if (auto committingState =
                 dynamic_cast<InputState::CommittingState*>(newState.get())) {
    handleCommittingState(*committingState, context);
    // Immediately transition to EmptyState after committing
    state_ = std::make_unique<InputState::EmptyState>();
  } else if (auto inputtingState =
                 dynamic_cast<InputState::InputtingState*>(newState.get())) {
    handleInputtingState(*inputtingState, context);
    state_ = std::move(newState);
  }
}

void FoxEngine::handleEmptyState(fcitx::InputContext* context) {
  context->inputPanel().reset();
  context->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
  context->updatePreedit();
}

void FoxEngine::handleCommittingState(
    const InputState::CommittingState& newState, fcitx::InputContext* context) {
  context->commitString(newState.commitString());
  context->inputPanel().reset();
  context->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
  context->updatePreedit();
}

void FoxEngine::handleInputtingState(const InputState::InputtingState& newState,
                                     fcitx::InputContext* context) {
  updateUI(newState, context);
}

void FoxEngine::updateUI(const InputState::InputtingState& newState,
                         fcitx::InputContext* context) {
  auto& inputPanel = context->inputPanel();
  inputPanel.reset();

  fcitx::Text preedit(newState.composingBuffer(),
                      fcitx::TextFormatFlag::Underline);
  if (newState.cursorIndex() <= newState.composingBuffer().length()) {
    preedit.setCursor(newState.cursorIndex());
  }
  inputPanel.setClientPreedit(preedit);
  const auto& candidates = newState.candidatesInCurrentPage();

  if (!candidates.empty()) {
    auto candidateList = std::make_unique<fcitx::CommonCandidateList>();
    candidateList->setLayoutHint(fcitx::CandidateLayoutHint::Vertical);
    auto keys = fcitx::Key::keyListFromString("1 2 3 4 5 6 7 8 9");
    candidateList->setSelectionKey(keys);

    for (size_t i = 0; i < candidates.size(); ++i) {
      std::string joined =
          candidates[i].displayText() + " " + candidates[i].description();
      candidateList->append(std::make_unique<FoxCandidate>(this, joined, i));
    }

    candidateList->setPageSize(candidates.size());
    int index = newState.selectedCandidateIndexInCurrentPage().value_or(0);
    if (index >= 0 && index < static_cast<int>(candidates.size())) {
      candidateList->setCursorIndex(index);
    }
    inputPanel.setCandidateList(std::move(candidateList));
  }

  context->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
  context->updatePreedit();
}

fcitx::AddonInstance* FoxAddonFactory::create(fcitx::AddonManager* manager) {
  return new FoxEngine(manager->instance());
}

}  // namespace McFoxIM

FCITX_ADDON_FACTORY(McFoxIM::FoxAddonFactory)
