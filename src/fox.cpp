#include "fox.h"
#include <fcitx/instance.h>
#include <fcitx/event.h>
#include <fcitx/inputmethodcontext.h>
#include <fcitx/candidate.h>
#include <fcitx/text.h>

namespace McFoxIM {

class FoxCandidate : public fcitx::CandidateWord {
public:
    FoxCandidate(FoxEngine *engine, std::string text, int index)
        : fcitx::CandidateWord(fcitx::Text(text)), engine_(engine), index_(index) {}

    void select(fcitx::InputMethodContext *context) const override {
        FCITX_UNUSED(context);
        engine_->selectCandidate(index_);
    }

private:
    FoxEngine *engine_;
    int index_;
};

FoxEngine::FoxEngine(fcitx::Instance *instance) : fcitx::InputMethodEngineV2(instance) {
    // Initialize components
    // Assuming data is in standard fcitx5 data dir "fox/data"
    std::string dataPath = fcitx::StandardPath::global().locate(fcitx::StandardPath::Type::PkgData, "fox/data");
    tableManager_ = std::make_unique<InputTableManager>(dataPath);
    
    // Set default table if available
    if (!tableManager_->availableTables().empty()) {
        tableManager_->setTable(tableManager_->availableTables()[0].name);
    }

    completer_ = std::make_unique<Completer>([this]() -> const InputTable& {
        return tableManager_->currentTable();
    });
    
    keyHandler_ = std::make_unique<KeyHandler>(*completer_);
    state_ = std::make_unique<InputState::EmptyState>();
    
    reloadConfig();
}

void FoxEngine::reloadConfig() {
    fcitx::Configuration::load(config_, "conf/fox.conf");
    if (tableManager_) {
        const char* names[] = {
            "TW_00", "TW_01", "TW_02", "TW_03", "TW_04", "TW_05", "TW_06", "TW_07", "TW_08", "TW_09", "TW_10",
            "TW_12", "TW_13", "TW_14", "TW_15", "TW_16", "TW_17", "TW_18", "TW_19", "TW_20",
            "TW_21", "TW_22", "TW_23", "TW_24", "TW_25", "TW_26", "TW_27", "TW_28", "TW_29", "TW_30",
            "TW_31", "TW_32", "TW_33", "TW_34", "TW_35", "TW_36", "TW_37", "TW_38", "TW_39", "TW_40",
            "TW_41", "TW_42"
        };
        int index = static_cast<int>(config_.table.value());
        if (index >= 0 && index < 42) { // 42 items total (0-41)
            tableManager_->setTable(names[index]);
        }
    }
}

void FoxEngine::reset(const fcitx::InputMethodEntry &entry, fcitx::InputMethodContext &context) {
    FCITX_UNUSED(entry);
    if (auto inputState = dynamic_cast<InputState::InputtingState*>(state_.get())) {
        if (!inputState->composingBuffer().empty()) {
            context.commitString(inputState->composingBuffer());
        }
    }
    enterState(std::make_unique<InputState::EmptyState>(), &context);
}

void FoxEngine::keyEvent(const fcitx::InputMethodEntry &entry, fcitx::KeyEvent &keyEvent) {
    FCITX_UNUSED(entry);
    if (keyEvent.isRelease()) {
        return;
    }

    auto context = keyEvent.inputContext();
    
    bool handled = keyHandler_->handle(keyEvent, *state_, 
        [this, context](std::unique_ptr<InputState::InputState> newState) {
            enterState(std::move(newState), context);
        },
        []() {
            // On error, maybe beep?
        }
    );

    if (handled) {
        keyEvent.accept();
    }
}

void FoxEngine::selectCandidate(int index, fcitx::InputMethodContext *context) {
    if (auto inputState = dynamic_cast<InputState::InputtingState*>(state_.get())) {
        const auto &candidates = inputState->candidatesInCurrentPage();
        if (index >= 0 && index < static_cast<int>(candidates.size())) {
            const auto &selectedCandidate = candidates[index];
            enterState(std::make_unique<InputState::CommittingState>(selectedCandidate.displayText() + " "), context);
        }
    }
}

void FoxEngine::enterState(std::unique_ptr<InputState::InputState> newState, fcitx::InputMethodContext *context) {
    if (dynamic_cast<InputState::EmptyState*>(newState.get())) {
        handleEmptyState(context);
        state_ = std::move(newState);
    } else if (auto committingState = dynamic_cast<InputState::CommittingState*>(newState.get())) {
        handleCommittingState(*committingState, context);
        // Immediately transition to EmptyState after committing
        state_ = std::make_unique<InputState::EmptyState>();
    } else if (auto inputtingState = dynamic_cast<InputState::InputtingState*>(newState.get())) {
        handleInputtingState(*inputtingState, context);
        state_ = std::move(newState);
    }
}

void FoxEngine::handleEmptyState(fcitx::InputMethodContext *context) {
    context->inputPanel().reset();
    context->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
    context->updatePreedit();
}

void FoxEngine::handleCommittingState(const InputState::CommittingState &newState, fcitx::InputMethodContext *context) {
    context->commitString(newState.commitString());
    context->inputPanel().reset();
    context->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
    context->updatePreedit();
}

void FoxEngine::handleInputtingState(const InputState::InputtingState &newState, fcitx::InputMethodContext *context) {
    updateUI(newState, context);
}

void FoxEngine::updateUI(const InputState::InputtingState &newState, fcitx::InputMethodContext *context) {
    // Update Preedit
    fcitx::Text preedit(newState.composingBuffer());
    if (newState.cursorIndex() <= newState.composingBuffer().length()) {
         // Fcitx5 handles cursor in preedit via setCursor? No, usually via TextFormat or InputMethodContext::updatePreedit
    }
    context->inputPanel().setClientPreedit(preedit);
    
    // Update Candidate List
    auto &inputPanel = context->inputPanel();
    inputPanel.reset();
    
    if (!newState.candidatesInCurrentPage().empty()) {
        auto candidateList = std::make_unique<fcitx::CommonCandidateList>();
        const auto &candidates = newState.candidatesInCurrentPage();
        for (size_t i = 0; i < candidates.size(); ++i) {
            // Use FoxCandidate for selection support
            candidateList->append(std::make_unique<FoxCandidate>(this, candidates[i].displayText(), i));
        }
        
        candidateList->setPage(newState.candidatePageIndex().value_or(0));
        candidateList->setPageSize(InputState::InputtingState::CANDIDATES_PER_PAGE);
        candidateList->setTotal(newState.candidates().size());
        
        inputPanel.setCandidateList(std::move(candidateList));
    }
    
    context->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
    context->updatePreedit();
}

fcitx::AddonInstance *FoxAddonFactory::create(fcitx::AddonManager *manager) {
    return new FoxEngine(manager->instance());
}

} // namespace McFoxIM

FCITX_ADDON_FACTORY(McFoxIM::FoxAddonFactory)
