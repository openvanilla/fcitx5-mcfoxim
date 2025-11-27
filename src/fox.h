#ifndef FOX_H_
#define FOX_H_

#include <fcitx/addonfactory.h>
#include <fcitx/inputmethodengine.h>
#include <memory>
#include "inputtablemanager.h"
#include "completer.h"
#include "keyhandler.h"
#include "inputstate.h"

namespace McFoxIM {

class FoxEngine : public fcitx::InputMethodEngineV2 {
public:
    FoxEngine(fcitx::Instance *instance);
    void keyEvent(const fcitx::InputMethodEntry &entry, fcitx::KeyEvent &keyEvent) override;
    void reset(const fcitx::InputMethodEntry &entry, fcitx::InputMethodContext &context) override;
    void selectCandidate(int index, fcitx::InputMethodContext *context);

private:
    void enterState(std::unique_ptr<InputState::InputState> newState, fcitx::InputMethodContext *context);
    void handleEmptyState(fcitx::InputMethodContext *context);
    void handleCommittingState(const InputState::CommittingState &newState, fcitx::InputMethodContext *context);
    void handleInputtingState(const InputState::InputtingState &newState, fcitx::InputMethodContext *context);
    void updateUI(const InputState::InputtingState &newState, fcitx::InputMethodContext *context);

    std::unique_ptr<InputTableManager> tableManager_;
    std::unique_ptr<Completer> completer_;
    std::unique_ptr<KeyHandler> keyHandler_;
    std::unique_ptr<InputState::InputState> state_;
};

class FoxAddonFactory : public fcitx::AddonFactory {
public:
    fcitx::AddonInstance *create(fcitx::AddonManager *manager) override;
};

} // namespace McFoxIM

#endif // FOX_H_
