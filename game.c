#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

game game_new(square *squares) {
    assert(squares);
    uint count = 0;
    // Memory allocation
    game g = (struct game_s *)malloc(sizeof(struct game_s));
    // In the first version the size of the game is always the same and there's
    // no wrapping nor unique mode
    g->length = DEFAULT_SIZE;
    g->width = DEFAULT_SIZE;
    g->wrappingOn = false;
    g->uniqueOn = false;
    g->undo_redo = queue_new();
    g->tab = (square **)malloc(g->length * sizeof(square *));

    for (uint i = 0; i < g->length; i++) {
        g->tab[i] = (square *)malloc(g->width * sizeof(square));
        for (uint j = 0; j < g->width; j++) {
            // Set each square at the initial state
            g->tab[i][j] = squares[count];
            count++;
        }
    }
    return g;
}

game game_new_empty(void) {
    // Memory allocation
    game g = (struct game_s *)malloc(sizeof(struct game_s));
    // In the first version the size of the game is always the same and there's
    // no wrapping nor unique mode
    g->length = DEFAULT_SIZE;
    g->width = DEFAULT_SIZE;
    g->wrappingOn = false;
    g->uniqueOn = false;
    g->undo_redo = queue_new();
    g->tab = (square **)malloc(g->length * sizeof(square *));

    for (uint i = 0; i < g->length; i++) {
        g->tab[i] = (square *)malloc(g->width * sizeof(square));
        for (uint j = 0; j < g->width; j++) {
            // Set each square at empty state
            g->tab[i][j] = S_EMPTY;
        }
    }
    return g;
}

game game_copy(cgame g) {
    assert(g);

    // Memory allocation
    game g2 =
        game_new_empty_ext(g->length, g->width, g->wrappingOn, g->uniqueOn);
    // g2->undo_redo = g->undo_redo;
    for (uint i = 0; i < g2->length; i++) {
        for (uint j = 0; j < g2->width; j++) {
            // copy each square from game g to game g2
            g2->tab[i][j] = g->tab[i][j];
        }
    }

    return g2;
}

bool game_equal(cgame g1, cgame g2) {
    assert(g1);
    assert(g2);
    if (g1->length != g2->length || g1->width != g2->width) return false;
    for (uint i = 0; i < g1->length; i++) {
        for (uint j = 0; j < g1->width; j++) {
            // Check for each square in game g1 and game g2 if they are equal
            if (g1->tab[i][j] != g2->tab[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void game_delete(game g) {
    assert(g);
    for (uint i = 0; i < g->length; i++) {
        if (g->tab[i] != NULL) {
            // frees the memory of each line
            free(g->tab[i]);
            g->tab[i] = NULL;
        }
    }
    if (g->tab != NULL) {
        free(g->tab);
    }
    if (g->undo_redo != NULL) {
        queue_free(g->undo_redo);
    }
    free(g);
}

void game_set_square(game g, uint i, uint j, square s) {
    assert(g);
    assert(i < g->length);
    assert(j < g->width);
    g->tab[i][j] = s;
}

square game_get_square(cgame g, uint i, uint j) {
    assert(g);
    assert(i < g->length);
    assert(j < g->width);
    return g->tab[i][j];
}

int game_get_number(cgame g, uint i, uint j) {
    if (g->tab[i][j] == S_ZERO || g->tab[i][j] == S_IMMUTABLE_ZERO) return 0;
    if (g->tab[i][j] == S_ONE || g->tab[i][j] == S_IMMUTABLE_ONE) return 1;
    return -1;
}

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist) {
    // we verify that the targeted square is in the grid
    // then we return the value of the targeted square
    // if the distance is bigger than the size of the grid the result will be
    // out the grid
    if (dist > DEFAULT_SIZE) return -1;
    if (dir == UP) {
        if (game_is_wrapping(g) == true)
            return game_get_square(g, (i + g->length - dist) % g->length, j);
        // i being an unsigned int, it can't be negative so we add to it the
        // size of the grid and we verify that i isn't lower than the size of
        // the grid if we soustract it by the distance
        if ((i + g->length) - dist < g->length) return -1;
        return g->tab[i - dist][j];
    }
    if (dir == DOWN) {
        if (game_is_wrapping(g) == true)
            return game_get_square(g, (i + g->length + dist) % g->length, j);
        if (i + dist >= g->length) return -1;
        return g->tab[i + dist][j];
    }
    if (dir == RIGHT) {
        if (game_is_wrapping(g) == true)
            return game_get_square(g, i, (j + g->length + dist) % g->length);
        if (j + dist >= g->width) return -1;
        return g->tab[i][j + dist];
    }
    if (dir == LEFT) {
        if (game_is_wrapping(g) == true)
            return game_get_square(g, i, (j + g->length - dist) % g->length);
        // j being an unsigned int, it can't be negative so we add to it the
        // size of the grid and we verify that j isn't lower than the size of
        // the grid if we soustract it by the distance
        if ((j + g->width) - dist < g->width) return -1;
        return g->tab[i][j - dist];
    }
    return -1;
}

int game_get_next_number(cgame g, uint i, uint j, direction dir, uint dist) {
    // we verify that the targeted square is in the grid
    // then we return the value of the targeted square
    // if the distance is bigger than the size of the grid the result will be
    // out the grid
    if (dir == UP) {
        if (game_is_wrapping(g) == true)
            return game_get_number(g, (i + g->length - dist) % g->length, j);
        // i being an unsigned int, it can't be negative so we add to it the
        // size of the grid and we verify that i isn't lower than the size of
        // the grid if we soustract it by the distance
        if ((i + g->length) - dist < g->length) return -1;
        return game_get_number(g, i - dist, j);
    } else if (dir == DOWN) {
        if (game_is_wrapping(g) == true)
            return game_get_number(g, (i + g->length + dist) % g->length, j);
        if (i + dist > g->length - 1) return -1;
        return game_get_number(g, i + dist, j);
    } else if (dir == RIGHT) {
        if (game_is_wrapping(g) == true)
            return game_get_number(g, i, (j + g->width + dist) % g->width);
        if (j + dist > g->width - 1) return -1;
        return game_get_number(g, i, j + dist);
    } else if (dir == LEFT) {
        if (game_is_wrapping(g) == true)
            return game_get_number(g, i, (j + g->width - dist) % g->width);
        // j being an unsigned int, it can't be negative so we add to it the
        // size of the grid and we verify that j isn't lower than the size of
        // the grid if we soustract it by the distance
        if ((j + g->width) - dist < g->width) return -1;
        return game_get_number(g, i, j - dist);
    }
    return -1;
}

bool game_is_empty(cgame g, uint i, uint j) {
    assert(g);
    if (g->tab[i][j] == S_EMPTY) return true;
    return false;
}

bool game_is_immutable(cgame g, uint i, uint j) {
    assert(g);
    if (g->tab[i][j] == S_IMMUTABLE_ONE || g->tab[i][j] == S_IMMUTABLE_ZERO)
        return true;
    return false;
}

bool game_Three_In_A_Row(cgame g, uint i, uint j, direction dir) {
    assert(g);
    if (game_get_next_number(g, i, j, dir, 1) == game_get_number(g, i, j) &&
        game_get_next_number(g, i, j, dir, 2) == game_get_number(g, i, j))
        return true;
    return false;
}

int game_has_error(cgame g, uint i, uint j) {
    assert(g);
    uint nb_of_squares = 0;
    if (game_get_square(g, i, j) == S_ONE ||
        game_get_square(g, i, j) == S_ZERO ||
        game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
        game_get_square(g, i, j) == S_IMMUTABLE_ZERO) {
        // 1st error : More than 3 (DEFAULT_SIZE/2) squares have the same value
        // in a row or a column Rows
        for (int k = 0; k < g->width; k++) {
            if (game_get_number(g, i, k) == game_get_number(g, i, j)) {
                nb_of_squares++;
            }
            if (nb_of_squares > g->width / 2) {
                return -1;
            }
        }
        nb_of_squares = 0;
        // Column
        for (int l = 0; l < g->length; l++) {
            if (game_get_number(g, l, j) == game_get_number(g, i, j)) {
                nb_of_squares++;
            }
            if (nb_of_squares > g->length / 2) {
                return -1;
            }
        }
        // 2nd error : there are more than 2 squares with same value in a row
        // First, we verify in a range of 2 around i,j
        if (game_Three_In_A_Row(g, i, j, UP) == true ||
            game_Three_In_A_Row(g, i, j, DOWN) == true ||
            game_Three_In_A_Row(g, i, j, LEFT) == true ||
            game_Three_In_A_Row(g, i, j, RIGHT) == true)
            return -2;

        // Secondly, if the wrapping isn't on, we verify in a range of 1 around
        // i,j
        if (game_is_wrapping(g) == false) {
            if (game_get_next_number(g, i, j, UP, 1) != -1)
                if (game_Three_In_A_Row(g, i - 1, j, DOWN) == true) return -2;
            if (game_get_next_number(g, i, j, DOWN, 1) != -1)
                if (game_Three_In_A_Row(g, i + 1, j, UP) == true) return -2;
            if (game_get_next_number(g, i, j, LEFT, 1) != -1)
                if (game_Three_In_A_Row(g, i, j - 1, RIGHT) == true) return -2;
            if (game_get_next_number(g, i, j, RIGHT, 1) != -1)
                if (game_Three_In_A_Row(g, i, j + 1, LEFT) == true) return -2;
        }

        // Thirdly, if the wrapping is on, we verify in a range of 1 around i,j
        // even if it goes out of the bounds of the game cause the game is
        // wrapping
        if (game_is_wrapping(g) == true) {
            if (game_Three_In_A_Row(g, i - 1, j, DOWN) == true) return -2;
            if (game_Three_In_A_Row(g, i + 1, j, UP) == true) return -2;
            if (game_Three_In_A_Row(g, i, j - 1, RIGHT) == true) return -2;
            if (game_Three_In_A_Row(g, i, j + 1, LEFT) == true) return -2;
        }
    }
    return 0;
}

bool game_check_move(cgame g, uint i, uint j, square s) {
    assert(g);
    // We check if the move we want to play verifies the 3 properties of a legal
    // move
    if (i >= g->length || j >= g->width) {
        return false;
    } else if (game_is_immutable(g, i, j)) {
        return false;
    } else if (s != S_ZERO && s != S_ONE && s != S_EMPTY) {
        return false;
    }
    return true;
}

void game_play_move(game g, uint i, uint j, square s) {
    assert(g);

    // We must clear redo moves
    while (g->undo_redo->tail->data->i != -1) {
        queue_pop_tail(g->undo_redo);
    }

    // We push in the queue the state of the square before we've played the move
    move_m m1 = create_move(i, j, game_get_square(g, i, j));
    queue_push_head(g->undo_redo, m1);

    game_set_square(g, i, j, s);
}

bool game_is_over(cgame g) {
    assert(g);
    int countRow;
    int countColumn;
    for (int i = 0; i < g->length; i++) {
        for (int j = 0; j < g->width; j++) {
            if (game_get_square(g, i, j) == S_EMPTY) {
                return false;
            } else if (game_has_error(g, i, j)) {
                return false;
            }
        }
        // if the unique mode is activated, we verify for each row if the other
        // rows in the grid are not identical
        if (game_is_unique(g) == true) {
            // We go through all the other rows from the one we are looking to
            // the last of the grid
            for (int k = i + 1; k < g->length; k++) {
                countRow = 0;
                for (int l = 0; l < g->width; l++) {
                    if (game_get_number(g, i, l) == game_get_number(g, k, l))
                        countRow++;
                    if (countRow >= g->width) return false;
                }
            }
        }
    }
    // if the unique mode is activated, we verify for each columns if the other
    // columns in the grid are not identical
    if (game_is_unique(g) == true) {
        for (int j = 0; j < g->width; j++) {
            // We go through all the other columns from the one we are looking
            // to the last of the grid
            for (int k = j + 1; k < g->width; k++) {
                countColumn = 0;
                for (int i = 0; i < g->length; i++) {
                    if (game_get_number(g, i, j) == game_get_number(g, i, k))
                        countColumn++;
                    if (countColumn >= g->length) return false;
                }
            }
        }
    }
    return true;
}

void game_restart(game g) {
    assert(g);
    // We go through the entire grid to set the squares to empty except the
    // immutable ones
    for (int i = 0; i < g->length; i++) {
        for (int j = 0; j < g->width; j++) {
            if (!game_is_immutable(g, i, j)) {
                game_set_square(g, i, j, S_EMPTY);
            }
        }
    }
}