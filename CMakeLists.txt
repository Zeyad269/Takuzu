cmake_minimum_required(VERSION 2.6)
project(game_test C)
include(CTest)

set(CMAKE_C_FLAGS "-Wall -std=c99")            # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage")   # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

add_library(game game.c game_aux.c game_ext.c queue.c game_tools.c)
link_directories(${CMAKE_SOURCE_DIR})


enable_testing()

add_executable(game_test_jolandry game_test_jolandry.c)
add_executable(game_test_rwach game_test_rwach.c)
add_executable(game_test_zsaidalichei game_test_zsaidalichei.c)

add_executable(game_text game_text.c)
add_executable(game_solve game_solve.c)

add_test(test_jolandry_dummy ./game_test_jolandry dummy)
add_test(test_jolandry_game_has_error ./game_test_jolandry has_error)
add_test(test_jolandry_game_check_move ./game_test_jolandry check_move)
add_test(test_jolandry_game_play_move ./game_test_jolandry play_move)
add_test(test_jolandry_game_is_over ./game_test_jolandry is_over)
add_test(test_jolandry_game_restart ./game_test_jolandry restart)
add_test(test_jolandry_game_default_solution ./game_test_jolandry game_default_solution)
add_test(test_jolandry_game_new_ext ./game_test_jolandry game_new_ext)
add_test(test_jolandry_game_new_empty_ext ./game_test_jolandry game_new_empty_ext)
add_test(test_jolandry_game_nb_rows ./game_test_jolandry game_nb_rows)
add_test(test_jolandry_game_nb_cols ./game_test_jolandry game_nb_cols)
add_test(test_jolandry_game_load ./game_test_jolandry game_load)

add_test(test_rwach_dummy ./game_test_rwach dummy)
add_test(test_rwach_game_get_square ./game_test_rwach get_square)
add_test(test_rwach_game_get_number ./game_test_rwach get_number)
add_test(test_rwach_game_get_next_square ./game_test_rwach get_next_square)
add_test(test_rwach_game_is_empty ./game_test_rwach is_empty)
add_test(test_rwach_game_get_next_number ./game_test_rwach get_next_number)
add_test(test_rwach_game_is_immutable ./game_test_rwach is_immutable)
add_test(test_rwach_game_print ./game_test_rwach game_print)
add_test(test_rwach_game_get_next_number_wrappingOn ./game_test_rwach get_next_number_wrappingOn)
add_test(test_rwach_game_is_wrapping ./game_test_rwach game_is_wrapping)
add_test(test_rwach_game_is_unique ./game_test_rwach game_is_unique)
add_test(test_rwach_game_is_over_uniqueOn ./game_test_rwach game_is_over_uniqueOn)
add_test(test_rwach_game_save ./game_test_rwach game_save)

add_test(test_zsaidalichei_dummy ./game_test_zsaidalichei dummy)
add_test(test_zsaidalichei_new ./game_test_zsaidalichei test_game_new)
add_test(test_zsaidalichei_new_empty ./game_test_zsaidalichei test_game_new_empty)
add_test(test_zsaidalichei_game_copy ./game_test_zsaidalichei test_game_copy)
add_test(test_zsaidalichei_game_equal ./game_test_zsaidalichei test_game_equal)
add_test(test_zsaidalichei_game_delete ./game_test_zsaidalichei test_game_delete)
add_test(test_zsaidalichei_set_square ./game_test_zsaidalichei test_game_set_square)
add_test(test_zsaidalichei_game_default ./game_test_zsaidalichei test_game_default)
add_test(test_zsaidalichei_game_redo ./game_test_zsaidalichei test_game_redo)
add_test(test_zsaidalichei_game_undo ./game_test_zsaidalichei test_game_undo)

# file copy
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# folder copy
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/build/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/)


target_link_libraries(game_text game)
target_link_libraries(game_test_rwach game)
target_link_libraries(game_test_jolandry game)
target_link_libraries(game_test_zsaidalichei game)
target_link_libraries(game_solve game)