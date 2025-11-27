#include "fox.h"

#include <fcitx-utils/log.h>
#include <fcitx-utils/standardpath.h>
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

FoxEngine::FoxEngine(fcitx::Instance* instance) : fcitx::InputMethodEngineV2() {
  // Assuming data is in standard fcitx5 data dir "fox/data"
  std::string dataPath = fcitx::StandardPath::global().locate(
      fcitx::StandardPath::Type::PkgData, "fox/data");
  FCITX_INFO() << "FoxEngine data path: " << dataPath;
  if (dataPath.empty()) {
#ifdef FOX_DATA_SOURCE_DIR
    dataPath = FOX_DATA_SOURCE_DIR;
    FCITX_INFO() << "Falling back to source data path: " << dataPath;
#endif
  }

  if (dataPath.empty()) {
    FCITX_ERROR() << "FoxEngine data path is empty. Cannot initialize input "
                     "table manager.";
    throw std::runtime_error("FoxEngine data path is empty.");
  }
  tableManager_ = std::make_unique<InputTableManager>(dataPath);
  // Set default table if available
  if (!tableManager_->availableTables().empty()) {
    tableManager_->setTable(tableManager_->availableTables()[0].name);
  }

  completer_ = std::make_unique<Completer>(
      [this]() -> const InputTable& { return tableManager_->currentTable(); });

  keyHandler_ = std::make_unique<KeyHandler>(*completer_);
  state_ = std::make_unique<InputState::EmptyState>();

  reloadConfig();
}

void FoxEngine::reloadConfig() {
  fcitx::readAsIni(config_, "conf/fox.conf");
  if (tableManager_) {
    const char* names[] = {
        "TW_00", "TW_01", "TW_02", "TW_03", "TW_04", "TW_05", "TW_06",
        "TW_07", "TW_08", "TW_09", "TW_10", "TW_12", "TW_13", "TW_14",
        "TW_15", "TW_16", "TW_17", "TW_18", "TW_19", "TW_20", "TW_21",
        "TW_22", "TW_23", "TW_24", "TW_25", "TW_26", "TW_27", "TW_28",
        "TW_29", "TW_30", "TW_31", "TW_32", "TW_33", "TW_34", "TW_35",
        "TW_36", "TW_37", "TW_38", "TW_39", "TW_40", "TW_41", "TW_42"};
    int index = static_cast<int>(config_.table.value());
    if (index >= 0 && index < 42) {  // 42 items total (0-41)
      tableManager_->setTable(names[index]);
    }
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
  FCITX_INFO() << "updateUI";
  FCITX_INFO() << "InputtingState: " << newState.composingBuffer();
  FCITX_INFO() << "InputtingState: " << newState.cursorIndex();
  FCITX_INFO() << "Candidates:";
  for (const auto& candidate : newState.candidates()) {
    FCITX_INFO() << "  - " << candidate.displayText();
  }

  // Update Preedit
  fcitx::Text preedit(newState.composingBuffer());
  if (newState.cursorIndex() <= newState.composingBuffer().length()) {
    // Fcitx5 handles cursor in preedit via setCursor? No, usually via
    // TextFormat or InputContext::updatePreedit
  }
  context->inputPanel().setClientPreedit(preedit);

  // Update Candidate List
  auto& inputPanel = context->inputPanel();
  inputPanel.reset();

  if (!newState.candidatesInCurrentPage().empty()) {
    auto candidateList = std::make_unique<fcitx::CommonCandidateList>();
    const auto& candidates = newState.candidatesInCurrentPage();
    for (size_t i = 0; i < candidates.size(); ++i) {
      // Use FoxCandidate for selection support
      candidateList->append(
          std::make_unique<FoxCandidate>(this, candidates[i].displayText(), i));
    }

    candidateList->setPage(newState.candidatePageIndex().value_or(0));
    candidateList->setPageSize(InputState::InputtingState::CANDIDATES_PER_PAGE);

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
