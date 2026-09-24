# GoogleTest is only used by the tests; the game itself has no third-party dependencies.
if(PRS_USE_SYSTEM_GTEST)
    find_package(GTest 1.10 CONFIG REQUIRED)
else()
    # A fixed release is downloaded and checked against its SHA-256 hash, so every build uses
    # exactly the same code.
    include(FetchContent)
    FetchContent_Declare(googletest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        URL_HASH SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c)

    # On Windows, make GoogleTest use the same C runtime library (/MD) as this project.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Do not add GoogleTest to this project's install rules.
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(assignment-IMCTests
    ConsoleAppTest.cpp    
    GameTest.cpp    
    MoveTest.cpp
    PlayerTest.cpp
    RulesTest.cpp
    TestHelpers.h)
target_link_libraries(assignment-IMCTests PRIVATE assignment-IMClib GTest::gmock_main)
prs_set_compile_options(assignment-IMCTests)

# Registers every test with CTest, so they show up in Visual Studio's Test Explorer and `ctest`.
include(GoogleTest)
gtest_discover_tests(assignment-IMCTests)
