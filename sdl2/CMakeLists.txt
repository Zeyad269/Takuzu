cmake_minimum_required(VERSION 3.0)

project ("game_sdl" LANGUAGES C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(game ../game.c ../game_aux.c ../game_ext.c ../queue.c ../game_tools.c)
link_directories(${CMAKE_SOURCE_DIR})

## find SDL2
include(SDL2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

## compilation rules
include_directories(res)
include_directories(../)
include_directories(${SDL2_ALL_INC})
add_executable(game_sdl game_sdl.c model.c)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m)
target_link_libraries(game_sdl game)

## copy useful data files in build dir
foreach(file "res/background.jpg" "res/asteroide.png" "res/asteroide2.png" "res/etoile-filante2.png" "res/etoile-filante.png" "res/question.png" "res/solution.png" "res/undo.png" "res/redo.png" "res/reset.png" "res/save.png" "res/Takuzu.png" "res/victory.png")
  file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)