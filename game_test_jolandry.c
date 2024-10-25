#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
#include "queue.h"

bool test_dummy() {
    char* if_dummy = "dummy";
    if (strcmp(if_dummy, "dummy") == 0) {
        return true;
    } else {
        return false;
    }
}

bool test_game_has_error() {
    game g = game_default_solution();
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            game_set_square(g, i, j, S_ZERO);
        }
    }
    if (game_has_error(g, 5, 5) == false) return false;
    return true;
}

bool test_game_check_move() {
    game g = game_default();

    if (game_check_move(g, 7, 1, S_ZERO) == true) {
        return false;
    }

    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_is_immutable(g, i, j) == true) {
                if (game_check_move(g, i, j, S_EMPTY) == true ||
                    game_check_move(g, i, j, S_ZERO) == true ||
                    game_check_move(g, i, j, S_ONE) == true ||
                    game_check_move(g, i, j, S_IMMUTABLE_ZERO) == true ||
                    game_check_move(g, i, j, S_IMMUTABLE_ONE) == true) {
                    return false;
                }
            } else {
                if (game_check_move(g, i, j, S_EMPTY) == false ||
                    game_check_move(g, i, j, S_ZERO) == false ||
                    game_check_move(g, i, j, S_ONE) == false ||
                    game_check_move(g, i, j, S_IMMUTABLE_ZERO) == true ||
                    game_check_move(g, i, j, S_IMMUTABLE_ONE) == true) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_game_play_move() {
    game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true, false);

    game_play_move(g, 0, 0, S_ONE);
    game_play_move(g, 1, 0, S_ZERO);

    // we verify if the square in the top of the stack is correct
    if (g->undo_redo->head->data->s != S_EMPTY) {
        return false;
    }

    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_is_immutable(g, i, j) == true) {
                game_play_move(g, i, j, S_ZERO);
                if (game_get_square(g, i, j) == S_ZERO) {
                    return false;
                }
                game_play_move(g, i, j, S_ONE);
                if (game_get_square(g, i, j) == S_ONE) {
                    return false;
                }
                game_play_move(g, i, j, S_EMPTY);
                if (game_get_square(g, i, j) == S_EMPTY) {
                    return false;
                }
            } else {
                game_play_move(g, i, j, S_ZERO);
                if (game_get_square(g, i, j) != S_ZERO) {
                    return false;
                }
                game_play_move(g, i, j, S_ONE);
                if (game_get_square(g, i, j) != S_ONE) {
                    return false;
                }
                game_play_move(g, i, j, S_EMPTY);
                if (game_get_square(g, i, j) != S_EMPTY) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool test_game_is_over() {
    game g = game_default_solution();

    if (game_is_over(g) == false) {
        return false;
    }
    return true;
}

bool test_game_restart() {
    game g = game_new_empty();

    for (uint k = 0; k < DEFAULT_SIZE; k++) {
        for (uint l = 0; l < DEFAULT_SIZE; l++) {
            if (k % 2 == 0) {
                game_set_square(g, k, l, S_ZERO);
            } else {
                game_set_square(g, k, l, S_ONE);
            }
        }
    }

    game_restart(g);

    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_is_empty(g, i, j) == false) {
                return false;
            }
        }
    }
    return true;
}

bool test_game_default_solution() {
    game g = game_default_solution();

    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_has_error(g, i, j) == true) {
                return false;
            }
        }
    }
    return true;
}

bool test_game_new_ext() {
    // Let's first define the parameters
    bool wrapping = false;
    bool unique = true;
    uint nb_rows = 4;
    uint nb_cols = 6;
    square squares[nb_rows * nb_cols];
    for (int i = 0; i < nb_rows * nb_cols; i++) {
        squares[i] = S_EMPTY;
    }
    // We then use a variable to store the game from the function we want to
    // test
    game g = game_new_ext(nb_rows, nb_cols, squares, wrapping, unique);
    // We check if the game's parameters are valid
    if (g == NULL) {
        return false;
    } else if (g->tab == NULL) {
        return false;
    } else if (g->uniqueOn != unique) {
        return false;
    } else if (g->wrappingOn != wrapping) {
        return false;
    } else if (g->length != nb_rows) {
        return false;
    } else if (g->width != nb_cols) {
        return false;
    } else if (g->undo_redo == NULL) {
        return false;
    }
    uint count = 0;
    for (int i = 0; i < g->length; i++) {
        for (int j = 0; j < g->width; j++) {
            if (game_get_square(g, i, j) != squares[count++]) {
                return false;
            }
        }
    }
    return true;
}

bool test_game_new_empty_ext() {
    // Let's first define the parameters
    bool wrapping = false;
    bool unique = true;
    uint nb_rows = 4;
    uint nb_cols = 6;

    // We then use a variable to store the game from the function we want to
    // test
    game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, unique);
    // We check if the game's parameters are valid
    if (g == NULL) {
        return false;
    } else if (g->uniqueOn != unique) {
        return false;
    } else if (g->wrappingOn != wrapping) {
        return false;
    } else if (g->length != nb_rows) {
        return false;
    } else if (g->width != nb_cols) {
        return false;
    } else if (g->undo_redo == NULL) {
        return false;
    }
    // We now verify if all the squares have the empty value
    for (int i = 0; i < g->length; i++) {
        for (int j = 0; j < g->width; j++) {
            if (game_get_square(g, i, j) != S_EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool test_game_nb_rows() {
    uint nb_rows = 5;
    uint nb_cols = 6;
    bool wrapping = true;
    bool unique = true;

    game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, unique);

    if (game_nb_rows(g) != g->length) {
        return false;
    }
    return true;
}

bool test_game_nb_cols() {
    uint nb_rows = 5;
    uint nb_cols = 6;
    bool wrapping = true;
    bool unique = true;

    game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, unique);

    if (game_nb_cols(g) != g->width) {
        return false;
    }
    return true;
}

bool test_game_load() {
    game g = game_new_empty_ext(6, 6, 0, 1);
    game_save(g, "test_load");
    game g2 = game_load("test_load");
    if (!game_equal(g, g2)) return false;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        return EXIT_FAILURE;
    }
    if (argc == 2) {
        bool verification = false;
        if (strcmp("dummy", argv[1]) == 0) {
            verification = test_dummy();
        } else if (strcmp("has_error", argv[1]) == 0) {
            verification = test_game_has_error();
        } else if (strcmp("check_move", argv[1]) == 0) {
            verification = test_game_check_move();
        } else if (strcmp("play_move", argv[1]) == 0) {
            verification = test_game_play_move();
        } else if (strcmp("is_over", argv[1]) == 0) {
            verification = test_game_is_over();
        } else if (strcmp("restart", argv[1]) == 0) {
            verification = test_game_restart();
        } else if (strcmp("game_default_solution", argv[1]) == 0) {
            verification = test_game_default_solution();
        } else if (strcmp("game_new_ext", argv[1]) == 0) {
            verification = test_game_new_ext();
        } else if (strcmp("game_new_empty_ext", argv[1]) == 0) {
            verification = test_game_new_empty_ext();
        } else if (strcmp("game_nb_rows", argv[1]) == 0) {
            verification = test_game_nb_rows();
        } else if (strcmp("game_nb_cols", argv[1]) == 0) {
            verification = test_game_nb_cols();
        } else if (strcmp("game_load", argv[1]) == 0) {
            verification = test_game_load();
        }
        if (verification == true) {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}