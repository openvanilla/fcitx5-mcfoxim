cmake_minimum_required(VERSION 3.10)
project(mcfoxim VERSION 0.1.2)

find_package(PkgConfig REQUIRED)
find_package(Fcitx5Core REQUIRED)
find_package(Fcitx5Utils REQUIRED)
MESSAGE(STATUS "Found Fcitx5Utils (found version \"${Fcitx5Utils_VERSION}\")")
if (Fcitx5Utils_VERSION VERSION_LESS "5.1.13")
    MESSAGE(STATUS "Use legacy FCITX5 API <standardpath.h>")
    add_compile_definitions(USE_LEGACY_FCITX5_API_STANDARDPATH=1)
endif()


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
install(FILES fox_TW_00.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_01.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_02.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_03.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_04.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_05.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_06.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_07.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_08.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_09.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_10.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_12.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_13.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_14.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_15.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_16.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_17.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_18.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_19.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_20.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_21.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_22.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_23.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_24.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_25.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_26.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_27.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_28.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_29.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_30.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_31.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_32.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_33.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_34.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_35.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_36.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_37.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_38.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_39.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_40.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_41.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")
install(FILES fox_TW_42.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/inputmethod")

