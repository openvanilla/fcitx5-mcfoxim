add_library(fox SHARED
    fox.cpp
    inputtable.cpp
    candidate.cpp
    completer.cpp
    inputstate.cpp
    keyhandler.cpp
    inputtablemanager.cpp
)

set_target_properties(fox PROPERTIES PREFIX "")

target_link_libraries(fox Fcitx5::Core Fcitx5::Config Fcitx5::Utils nlohmann_json::nlohmann_json)

install(TARGETS fox DESTINATION "${FCITX_INSTALL_LIBDIR}/fcitx5")
install(FILES fox.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon")
install(FILES fox-input.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
