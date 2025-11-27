#ifndef FOX_H_
#define FOX_H_

#include <fcitx/addonfactory.h>
#include <fcitx/inputmethodengine.h>

#include <memory>

#include "completer.h"
#include "config.h"
#include "inputstate.h"
#include "inputtablemanager.h"
#include "keyhandler.h"

namespace fcitx {
class Instance;
}

namespace McFoxIM {

class FoxEngine : public fcitx::InputMethodEngineV2 {
 public:
  FoxEngine(fcitx::Instance* instance);
  void keyEvent(const fcitx::InputMethodEntry& entry,
                fcitx::KeyEvent& keyEvent) override;
  void reset(const fcitx::InputMethodEntry& entry,
             fcitx::InputContext& context);
  void selectCandidate(int index, fcitx::InputContext* context);
  void reloadConfig() override;

 private:
  void enterState(std::unique_ptr<InputState::InputState> newState,
                  fcitx::InputContext* context);
  void handleEmptyState(fcitx::InputContext* context);
  void handleCommittingState(const InputState::CommittingState& newState,
                             fcitx::InputContext* context);
  void handleInputtingState(const InputState::InputtingState& newState,
                            fcitx::InputContext* context);
  void updateUI(const InputState::InputtingState& newState,
                fcitx::InputContext* context);

  std::unique_ptr<InputTableManager> tableManager_;
  std::unique_ptr<Completer> completer_;
  std::unique_ptr<KeyHandler> keyHandler_;
  std::unique_ptr<InputState::InputState> state_;
  FoxConfig config_;
};

class FoxAddonFactory : public fcitx::AddonFactory {
 public:
  fcitx::AddonInstance* create(fcitx::AddonManager* manager) override;
};

}  // namespace McFoxIM

#endif  // FOX_H_
