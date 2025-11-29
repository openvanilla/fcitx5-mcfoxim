find_package(Fcitx5Core REQUIRED)
find_package(Fcitx5Utils REQUIRED)

add_executable(test_completer test_completer.cpp
    ../src/completer.cpp
    ../src/inputtable.cpp
    ../src/candidate.cpp
)

target_link_libraries(test_completer
    Fcitx5::Core
    Fcitx5::Utils
    nlohmann_json::nlohmann_json
)

target_include_directories(test_completer PRIVATE ../src)

add_executable(test_inputstate test_inputstate.cpp
    ../src/inputstate.cpp
    ../src/candidate.cpp
)
target_link_libraries(test_inputstate
    Fcitx5::Core
    Fcitx5::Utils
)
target_include_directories(test_inputstate PRIVATE ../src)

add_executable(test_keyhandler test_keyhandler.cpp
    ../src/keyhandler.cpp
    ../src/completer.cpp
    ../src/inputtable.cpp
    ../src/candidate.cpp
    ../src/inputstate.cpp
)
target_link_libraries(test_keyhandler
    Fcitx5::Core
    Fcitx5::Utils
    nlohmann_json::nlohmann_json
)
target_include_directories(test_keyhandler PRIVATE ../src)

add_test(NAME test_completer COMMAND test_completer)
add_test(NAME test_inputstate COMMAND test_inputstate)
add_test(NAME test_keyhandler COMMAND test_keyhandler)
