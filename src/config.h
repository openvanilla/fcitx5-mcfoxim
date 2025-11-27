#ifndef CONFIG_H_
#define CONFIG_H_

#include <fcitx-config/configuration.h>
#include <fcitx-config/enum.h>
#include <fcitx-config/iniparser.h>

namespace McFoxIM {

enum class TableOption {
    TW_00, TW_01, TW_02, TW_03, TW_04, TW_05, TW_06, TW_07, TW_08, TW_09, TW_10,
    TW_12, TW_13, TW_14, TW_15, TW_16, TW_17, TW_18, TW_19, TW_20,
    TW_21, TW_22, TW_23, TW_24, TW_25, TW_26, TW_27, TW_28, TW_29, TW_30,
    TW_31, TW_32, TW_33, TW_34, TW_35, TW_36, TW_37, TW_38, TW_39, TW_40,
    TW_41, TW_42
};

FCITX_CONFIG_ENUM(TableOption,
    TW_00, "南勢阿美語",
    TW_01, "秀姑巒阿美語",
    TW_02, "海岸阿美語",
    TW_03, "馬蘭阿美語",
    TW_04, "恆春阿美語",
    TW_05, "賽考利克泰雅語",
    TW_06, "澤敖利泰雅語",
    TW_07, "汶水泰雅語",
    TW_08, "萬大泰雅語",
    TW_09, "四季泰雅語",
    TW_10, "宜蘭澤敖利泰雅語",
    TW_12, "賽夏語",
    TW_13, "邵語",
    TW_14, "都達賽德克語",
    TW_15, "德固達雅賽德克語",
    TW_16, "德鹿谷賽德克語",
    TW_17, "卓群布農語",
    TW_18, "卡群布農語",
    TW_19, "丹群布農語",
    TW_20, "巒群布農語",
    TW_21, "郡群布農語",
    TW_22, "東排灣語",
    TW_23, "北排灣語",
    TW_24, "中排灣語",
    TW_25, "南排灣語",
    TW_26, "東魯凱語",
    TW_27, "霧台魯凱語",
    TW_28, "大武魯凱語",
    TW_29, "多納魯凱語",
    TW_30, "茂林魯凱語",
    TW_31, "萬山魯凱語",
    TW_32, "太魯閣語",
    TW_33, "噶瑪蘭語",
    TW_34, "鄒語",
    TW_35, "卡那卡那富語",
    TW_36, "拉阿魯哇語",
    TW_37, "南王卑南語",
    TW_38, "知本卑南語",
    TW_39, "西群卑南語",
    TW_40, "建和卑南語",
    TW_41, "雅美語",
    TW_42, "撒奇萊雅語"
);

FCITX_CONFIG_STRUCT(FoxConfig,
    fcitx::Option<TableOption> table{this, "Table", "The input table to use", TableOption::TW_00};
);

} // namespace McFoxIM

#endif // CONFIG_H_
