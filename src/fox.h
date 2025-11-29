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

#ifndef FOX_H_
#define FOX_H_

#include <fcitx-config/configuration.h>
#include <fcitx-config/enum.h>
#include <fcitx-config/iniparser.h>
#include <fcitx-utils/i18n.h>
#include <fcitx/addonfactory.h>
#include <fcitx/inputmethodengine.h>

#include <memory>

#include "completer.h"
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
  void activate(const fcitx::InputMethodEntry& entry,
                fcitx::InputContextEvent& event) override;
  void keyEvent(const fcitx::InputMethodEntry& entry,
                fcitx::KeyEvent& keyEvent) override;
  void reset(const fcitx::InputMethodEntry& entry,
             fcitx::InputContext& context);
  void selectCandidate(int index, fcitx::InputContext* context);

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

  std::string currentTableName_;
  std::unique_ptr<InputTableManager> tableManager_;
  std::unique_ptr<Completer> completer_;
  std::unique_ptr<KeyHandler> keyHandler_;
  std::unique_ptr<InputState::InputState> state_;
};

class FoxAddonFactory : public fcitx::AddonFactory {
 public:
  fcitx::AddonInstance* create(fcitx::AddonManager* manager) override;
};

}  // namespace McFoxIM

#endif  // FOX_H_
