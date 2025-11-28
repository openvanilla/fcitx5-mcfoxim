cmake_minimum_required(VERSION 3.10)
project(mcfoxim VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
find_package(Fcitx5Core REQUIRED)
find_package(Fcitx5Utils REQUIRED)
# MESSAGE(STATUS "Found Fcitx5Utils (found version \"${Fcitx5Utils_VERSION}\")")
# if (Fcitx5Utils_VERSION VERSION_LESS "5.1.13")
#     MESSAGE(STATUS "Use legacy FCITX5 API <standardpath.h>")
#     add_compile_definitions(USE_LEGACY_FCITX5_API_STANDARDPATH=1)
# endif()


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
