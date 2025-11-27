#ifndef CONFIG_H_
#define CONFIG_H_

#include <fcitx-config/configuration.h>
#include <fcitx-config/enum.h>
#include <fcitx-config/iniparser.h>

#include <fcitx-utils/i18n.h>

namespace McFoxIM {

enum class TableOption {
    TW_00, TW_01, TW_02, TW_03, TW_04, TW_05, TW_06, TW_07, TW_08, TW_09, TW_10,
    TW_12, TW_13, TW_14, TW_15, TW_16, TW_17, TW_18, TW_19, TW_20,
    TW_21, TW_22, TW_23, TW_24, TW_25, TW_26, TW_27, TW_28, TW_29, TW_30,
    TW_31, TW_32, TW_33, TW_34, TW_35, TW_36, TW_37, TW_38, TW_39, TW_40,
    TW_41, TW_42
};

FCITX_CONFIG_ENUM_NAME_WITH_I18N(TableOption,
    TableOption::TW_00, N_("南勢阿美語"),
    TableOption::TW_01, N_("秀姑巒阿美語"),
    TableOption::TW_02, N_("海岸阿美語"),
    TableOption::TW_03, N_("馬蘭阿美語"),
    TableOption::TW_04, N_("恆春阿美語"),
    TableOption::TW_05, N_("賽考利克泰雅語"),
    TableOption::TW_06, N_("澤敖利泰雅語"),
    TableOption::TW_07, N_("汶水泰雅語"),
    TableOption::TW_08, N_("萬大泰雅語"),
    TableOption::TW_09, N_("四季泰雅語"),
    TableOption::TW_10, N_("宜蘭澤敖利泰雅語"),
    TableOption::TW_12, N_("賽夏語"),
    TableOption::TW_13, N_("邵語"),
    TableOption::TW_14, N_("都達賽德克語"),
    TableOption::TW_15, N_("德固達雅賽德克語"),
    TableOption::TW_16, N_("德鹿谷賽德克語"),
    TableOption::TW_17, N_("卓群布農語"),
    TableOption::TW_18, N_("卡群布農語"),
    TableOption::TW_19, N_("丹群布農語"),
    TableOption::TW_20, N_("巒群布農語"),
    TableOption::TW_21, N_("郡群布農語"),
    TableOption::TW_22, N_("東排灣語"),
    TableOption::TW_23, N_("北排灣語"),
    TableOption::TW_24, N_("中排灣語"),
    TableOption::TW_25, N_("南排灣語"),
    TableOption::TW_26, N_("東魯凱語"),
    TableOption::TW_27, N_("霧台魯凱語"),
    TableOption::TW_28, N_("大武魯凱語"),
    TableOption::TW_29, N_("多納魯凱語"),
    TableOption::TW_30, N_("茂林魯凱語"),
    TableOption::TW_31, N_("萬山魯凱語"),
    TableOption::TW_32, N_("太魯閣語"),
    TableOption::TW_33, N_("噶瑪蘭語"),
    TableOption::TW_34, N_("鄒語"),
    TableOption::TW_35, N_("卡那卡那富語"),
    TableOption::TW_36, N_("拉阿魯哇語"),
    TableOption::TW_37, N_("南王卑南語"),
    TableOption::TW_38, N_("知本卑南語"),
    TableOption::TW_39, N_("西群卑南語"),
    TableOption::TW_40, N_("建和卑南語"),
    TableOption::TW_41, N_("雅美語"),
    TableOption::TW_42, N_("撒奇萊雅語")
);

FCITX_CONFIG_STRUCT(FoxConfig,
    fcitx::Option<TableOption> table{this, "Table", "The input table to use", TableOption::TW_00};
);

} // namespace McFoxIM

#endif // CONFIG_H_
