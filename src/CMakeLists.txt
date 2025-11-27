add_library(fox MODULE
    fox.cpp
    fox.h
    inputtable.cpp
    inputtable.h
    candidate.cpp
    candidate.h
    completer.cpp
    completer.h
    inputstate.cpp
    inputstate.h
    keyhandler.cpp
    keyhandler.h
)

target_link_libraries(fox Fcitx5::Core Fcitx5::Module nlohmann_json::nlohmann_json)

install(TARGETS fox DESTINATION "${FCITX_INSTALL_LIBDIR}/fcitx5")
install(FILES fox.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon")
install(FILES fox-input.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
