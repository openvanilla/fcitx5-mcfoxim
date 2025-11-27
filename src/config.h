#ifndef CONFIG_H_
#define CONFIG_H_

#include <fcitx-config/configuration.h>
#include <fcitx-config/enum.h>
#include <fcitx-config/iniparser.h>

#include <fcitx-utils/i18n.h>

namespace McFoxIM {

enum TableOption {
    TW_00, TW_01, TW_02, TW_03, TW_04, TW_05, TW_06, TW_07, TW_08, TW_09, TW_10,
    TW_12, TW_13, TW_14, TW_15, TW_16, TW_17, TW_18, TW_19, TW_20,
    TW_21, TW_22, TW_23, TW_24, TW_25, TW_26, TW_27, TW_28, TW_29, TW_30,
    TW_31, TW_32, TW_33, TW_34, TW_35, TW_36, TW_37, TW_38, TW_39, TW_40,
    TW_41, TW_42
};

/*
FCITX_CONFIG_ENUM_NAME_WITH_I18N(TableOption,
    TW_00, N_("南勢阿美語"),
    // ...
);
*/

FCITX_CONFIGURATION(FoxConfig,
    fcitx::Option<int> table{this, "Table", "The input table to use", 0};
);

} // namespace McFoxIM

#endif // CONFIG_H_
