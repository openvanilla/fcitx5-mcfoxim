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

add_test(NAME test_completer COMMAND test_completer)
