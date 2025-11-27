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

enum class TableOption {
  TW_00,
  TW_01,
  TW_02,
  TW_03,
  TW_04,
  TW_05,
  TW_06,
  TW_07,
  TW_08,
  TW_09,
  TW_10,
  TW_12,
  TW_13,
  TW_14,
  TW_15,
  TW_16,
  TW_17,
  TW_18,
  TW_19,
  TW_20,
  TW_21,
  TW_22,
  TW_23,
  TW_24,
  TW_25,
  TW_26,
  TW_27,
  TW_28,
  TW_29,
  TW_30,
  TW_31,
  TW_32,
  TW_33,
  TW_34,
  TW_35,
  TW_36,
  TW_37,
  TW_38,
  TW_39,
  TW_40,
  TW_41,
  TW_42
};

FCITX_CONFIG_ENUM_NAME_WITH_I18N(
    TableOption, N_("南勢阿美語"), N_("秀姑巒阿美語"), N_("海岸阿美語"),
    N_("馬蘭阿美語"), N_("恆春阿美語"), N_("賽考利克泰雅語"),
    N_("澤敖利泰雅語"), N_("汶水泰雅語"), N_("萬大泰雅語"), N_("四季泰雅語"),
    N_("宜蘭澤敖利泰雅語"), N_("賽夏語"), N_("邵語"), N_("都達賽德克語"),
    N_("德固達雅賽德克語"), N_("德鹿谷賽德克語"), N_("卓群布農語"),
    N_("卡群布農語"), N_("丹群布農語"), N_("巒群布農語"), N_("郡群布農語"),
    N_("東排灣語"), N_("北排灣語"), N_("中排灣語"), N_("南排灣語"),
    N_("東魯凱語"), N_("霧台魯凱語"), N_("大武魯凱語"), N_("多納魯凱語"),
    N_("茂林魯凱語"), N_("萬山魯凱語"), N_("太魯閣語"), N_("噶瑪蘭語"),
    N_("鄒語"), N_("卡那卡那富語"), N_("拉阿魯哇語"), N_("南王卑南語"),
    N_("知本卑南語"), N_("西群卑南語"), N_("建和卑南語"), N_("雅美語"),
    N_("撒奇萊雅語"));

FCITX_CONFIGURATION(
    FoxConfig,
    // Keyboard layout: standard, eten, etc.
    fcitx::OptionWithAnnotation<TableOption, TableOptionI18NAnnotation> table{
        this, "table", _("Table"), TableOption::TW_00};);

class FoxEngine : public fcitx::InputMethodEngineV2 {
 public:
  FoxEngine(fcitx::Instance* instance);
  void keyEvent(const fcitx::InputMethodEntry& entry,
                fcitx::KeyEvent& keyEvent) override;
  void reset(const fcitx::InputMethodEntry& entry,
             fcitx::InputContext& context);
  void selectCandidate(int index, fcitx::InputContext* context);
  const fcitx::Configuration* getConfig() const override;
  void setConfig(const fcitx::RawConfig& config) override;
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
