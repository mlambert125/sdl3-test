cmake_minimum_required(VERSION 3.5)
project(sdl3 VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/lua EXCLUDE_FROM_ALL)

set(SourceFiles
    src/main.c
    src/game-state.c
    src/scene-title.c
    src/scene-game.c
)

add_executable(${PROJECT_NAME} ${SourceFiles})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf lua_static)
