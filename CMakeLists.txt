cmake_minimum_required(VERSION 3.10)

project(
    team4_game_engine
    VERSION 0.1.0
    DESCRIPTION "simple engine with network"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

option(TEAM4_GAME_ENGINE_BUILD_TESTS "build tests alongside the main library" ON)
option(TEAM4_GAME_ENGINE_BUILD_EXAMPLES "build examples alongside the main library" ON)

# add extern folder for external librairies
add_subdirectory(extern)

# add main library code
add_subdirectory(team4_game_engine)

# add game code
add_subdirectory(games)

# add text code
if(TEAM4_GAME_ENGINE_BUILD_TESTS)
    add_subdirectory(team4_game_engine_test)
endif()
# add examples code
if(TEAM4_GAME_ENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()