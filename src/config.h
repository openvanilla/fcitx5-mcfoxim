#ifndef CONFIG_H_
#define CONFIG_H_

#include <fcitx/config/configuration.h>
#include <fcitx/config/option.h>

namespace McFoxIM {

enum class TableOption {
    TW_01,
    TW_02,
    TW_03,
    TW_04,
    TW_05,
    TW_06,
    TW_07,
    TW_08,
    TW_09,
    TW_10
};

FCITX_CONFIG_ENUM(TableOption,
    TW_01,
    TW_02,
    TW_03,
    TW_04,
    TW_05,
    TW_06,
    TW_07,
    TW_08,
    TW_09,
    TW_10
);

FCITX_CONFIG_STRUCT(FoxConfig,
    fcitx::Option<TableOption> table{this, "Table", "The input table to use", TableOption::TW_01};
);

} // namespace McFoxIM

#endif // CONFIG_H_
