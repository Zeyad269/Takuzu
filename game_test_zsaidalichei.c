#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

bool test_dummy(char* test) {
    if (strcmp(test, "dummy") == 0) {
        return true;
    }
    return false;
}

bool test_game_new(void) {
    /* Creates an array with only empty cases */
    square squares[DEFAULT_SIZE * DEFAULT_SIZE];
    for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
        squares[i] = S_EMPTY;
    }

    /* Creates a game with only empty cases */
    game g = game_new(squares);

    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g, i, j) != S_EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool test_game_new_empty(void) {
    /* Creates a game with only empty cases */
    game g = game_new_empty();
    if (g == NULL) {
        return false;
    }
    /* Test if all the cases are empty */
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g, i, j) != S_EMPTY) {
                game_delete(g);
                return false;
            }
        }
    }
    return true;
}

bool test_game_copy(void) {
    /* Creates a default game and makes a copy of it */
    game g = game_new_empty_ext(4, 5, true, false);
    game cg = game_copy(g);
    if (cg == NULL) {
        return false;
    }

    /* Test if all the options of the copied game are the same than the original
     * one */
    if (cg->uniqueOn != false || cg->wrappingOn != true) {
        game_delete(g);
        game_delete(cg);
        return false;
    }

    if (cg->length != 4 || cg->width != 5) {
        game_delete(g);
        game_delete(cg);
        return false;
    }

    /* Test if all the cases of the copied game are the same than the original
     * one */
    for (int i = 0; i < g->length; i++) {
        for (int j = 0; j < g->width; j++) {
            if (game_get_square(g, i, j) != game_get_square(cg, i, j)) {
                game_delete(g);
                game_delete(cg);
                return false;
            }
        }
    }

    game_delete(g);
    game_delete(cg);
    return true;
}

bool test_game_equal(void) {
    game g1 = game_new_empty();
    game g2 = game_new_empty();

    game_set_square(g1, 0, 0, S_IMMUTABLE_ONE);
    game_set_square(g2, 0, 0, S_ONE);
    if (game_equal(g1, g2) == true) return false;

    game_set_square(g1, 0, 0, S_IMMUTABLE_ZERO);
    game_set_square(g2, 0, 0, S_ZERO);
    if (game_equal(g1, g2) == true) return false;

    game_set_square(g1, 0, 0, S_ONE);
    game_set_square(g2, 0, 0, S_ZERO);
    if (game_equal(g1, g2) == true) return false;

    game_set_square(g1, 0, 0, S_ZERO);
    game_set_square(g2, 0, 0, S_ZERO);
    if (game_equal(g1, g2) == false) return false;

    game_set_square(g1, 0, 0, S_ONE);
    game_set_square(g2, 0, 0, S_ONE);
    if (game_equal(g1, g2) == false) return false;

    game_set_square(g1, 0, 0, S_IMMUTABLE_ONE);
    game_set_square(g2, 0, 0, S_IMMUTABLE_ONE);
    if (game_equal(g1, g2) == false) return false;

    game_set_square(g1, 0, 0, S_IMMUTABLE_ZERO);
    game_set_square(g2, 0, 0, S_IMMUTABLE_ZERO);
    if (game_equal(g1, g2) == false) return false;

    return true;
}

bool test_game_delete(void) {
    game g = game_default();
    game_delete(g);
    return true;
}

bool test_game_set_square(void) {
    game g = game_new_empty();

    game_set_square(g, 0, 0, S_ONE);
    game_set_square(g, 1, 0, S_ONE);
    game_set_square(g, 1, 0, S_ZERO);
    game_set_square(g, 5, 5, S_IMMUTABLE_ZERO);

    /* Test if all the values have been set correctly */
    if (game_get_square(g, 0, 0) == S_ONE &&
        game_get_square(g, 1, 0) == S_ZERO &&
        game_get_square(g, 5, 5) == S_IMMUTABLE_ZERO) {
        game_delete(g);
        return true;
    }
    game_delete(g);
    return false;
}

bool test_game_default(void) {
    game g = game_default();

    /*Test if all the immutables cases of the game are correctly set*/
    if (game_get_square(g, 0, 1) == S_IMMUTABLE_ONE &&
        game_get_square(g, 3, 2) == S_IMMUTABLE_ONE &&
        game_get_square(g, 4, 2) == S_IMMUTABLE_ONE &&
        game_get_square(g, 0, 2) == S_IMMUTABLE_ZERO &&
        game_get_square(g, 2, 1) == S_IMMUTABLE_ZERO &&
        game_get_square(g, 2, 4) == S_IMMUTABLE_ZERO &&
        game_get_square(g, 3, 1) == S_IMMUTABLE_ZERO &&
        game_get_square(g, 4, 5) == S_IMMUTABLE_ZERO &&
        game_get_square(g, 5, 5) == S_IMMUTABLE_ZERO) {
        /* Replacing all of them by empty cases */
        game_set_square(g, 0, 1, S_EMPTY);
        game_set_square(g, 3, 2, S_EMPTY);
        game_set_square(g, 4, 2, S_EMPTY);
        game_set_square(g, 0, 2, S_EMPTY);
        game_set_square(g, 2, 1, S_EMPTY);
        game_set_square(g, 2, 4, S_EMPTY);
        game_set_square(g, 3, 1, S_EMPTY);
        game_set_square(g, 4, 5, S_EMPTY);
        game_set_square(g, 5, 5, S_EMPTY);
    }

    /* Test if all the remaining cases are empty */
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g, i, j) != S_EMPTY) {
                game_delete(g);
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

bool test_game_undo(void) {
    game g = game_default();
    game_play_move(g, 0, 0, S_ONE);
    game_undo(g);

    /* Test if undo command has cancelled the last move */
    if (game_get_square(g, 0, 0) != S_EMPTY) {
        return false;
    }

    game_play_move(g, 1, 0, S_ZERO);
    game_play_move(g, 1, 0, S_ONE);
    game_undo(g);

    /* Test if undo command conserve the order of the moves */
    if (game_get_square(g, 1, 0) != S_ZERO &&
        game_get_square(g, 0, 0) != S_ONE) {
        return false;
    }
    return true;
}

bool test_game_redo(void) {
    game g = game_new_empty();
    game_play_move(g, 0, 0, S_ONE);
    game_undo(g);
    game_redo(g);
    if (game_get_square(g, 0, 0) != S_ONE) {
        return false;
    }

    game_play_move(g, 1, 0, S_ZERO);
    game_play_move(g, 1, 0, S_ONE);
    game_undo(g);
    game_undo(g);
    game_redo(g);
    if (game_get_square(g, 1, 0) != S_EMPTY &&
        game_get_square(g, 0, 0) != S_ONE) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (test_dummy(argv[1]) == true) {
            return EXIT_SUCCESS;
        }

        bool Ok = false;

        if (strcmp(argv[1], "test_game_new_empty") == 0) {
            Ok = test_game_new_empty();
        } else if (strcmp(argv[1], "test_game_new") == 0) {
            Ok = test_game_new();
        } else if (strcmp(argv[1], "test_game_copy") == 0) {
            Ok = test_game_copy();
        } else if (strcmp(argv[1], "test_game_equal") == 0) {
            Ok = test_game_equal();
        } else if (strcmp(argv[1], "test_game_delete") == 0) {
            Ok = test_game_delete();
        } else if (strcmp(argv[1], "test_game_set_square") == 0) {
            Ok = test_game_set_square();
        } else if (strcmp(argv[1], "test_game_default") == 0) {
            Ok = test_game_default();
        } else if (strcmp(argv[1], "test_game_undo") == 0) {
            Ok = test_game_undo();
        } else if (strcmp(argv[1], "test_game_redo") == 0) {
            Ok = test_game_redo();
        }

        if (Ok) {
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}