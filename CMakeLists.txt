cmake_minimum_required(VERSION 3.16...3.31)

project(PaperRockScissors
    VERSION 1.0.0
    DESCRIPTION "Paper-Rock-Scissors console game: a player against the computer"
    LANGUAGES CXX)

option(PRS_BUILD_TESTS "Build the unit tests (downloads GoogleTest on the first configure)" ON)
option(PRS_USE_SYSTEM_GTEST
    "Use an installed GoogleTest (e.g. a Linux package) instead of downloading it" OFF)

set(CMAKE_CXX_EXTENSIONS OFF)

# Strict warnings and standard conformance for this project's code (not for third-party code).
function(prs_set_compile_options target)
    target_compile_features(${target} PUBLIC cxx_std_17)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    endif()
endfunction()

# The game logic is a library, so the game and the unit tests are built from the same code.
add_library(PaperRockScissorsLib STATIC
    src/Game.cpp
    src/Game.h    
    src/Move.cpp
    src/Move.h
    src/Player.cpp
    src/Player.h
    src/Rules.cpp
    src/Rules.h)
target_include_directories(PaperRockScissorsLib PUBLIC src)
prs_set_compile_options(PaperRockScissorsLib)

#add_executable(PaperRockScissors src/main.cpp)
#target_link_libraries(PaperRockScissors PRIVATE PaperRockScissorsLib)
#prs_set_compile_options(PaperRockScissors)

# Makes the game the startup project when a Visual Studio solution (.sln) is generated.
#set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT PaperRockScissors)

if(PRS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
