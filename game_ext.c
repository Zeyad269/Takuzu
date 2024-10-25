#include "game_ext.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_struct.h"
#include "queue.h"

game game_new_ext(uint nb_rows, uint nb_cols, square *squares, bool wrapping,
                  bool unique) {
    assert(squares);
    uint count = 0;
    // Memory allocation
    game g = (struct game_s *)malloc(sizeof(struct game_s));
    // In the second version the size of the game is not necesserally the same
    // and there may be wrapping and unique options on.
    g->length = nb_rows;
    g->width = nb_cols;
    g->wrappingOn = wrapping;
    g->uniqueOn = unique;
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

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool unique) {
    // Memory allocation
    game g = (struct game_s *)malloc(sizeof(struct game_s));
    // In the second version the size of the game is not necesserally the same
    // and there may be wrapping and unique options on.
    g->length = nb_rows;
    g->width = nb_cols;
    g->wrappingOn = wrapping;
    g->uniqueOn = unique;
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

uint game_nb_rows(cgame g) { return g->length; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrappingOn; }

bool game_is_unique(cgame g) { return g->uniqueOn; }

void game_undo(game g) {
    assert(g);

    // We pop from the head of the queue
    move_m undo = queue_pop_head(g->undo_redo);

    // We add in the redo stack the former square
    move_m m =
        create_move(undo->i, undo->j, game_get_square(g, undo->i, undo->j));
    queue_push_tail(g->undo_redo, m);

    // We set it in the game
    game_set_square(g, undo->i, undo->j, undo->s);
}

void game_redo(game g) {
    assert(g);

    // We pop from the tail of the queue the state of the square before we've
    // redo
    move_m redo = queue_pop_tail(g->undo_redo);

    // We set it in the game
    game_set_square(g, redo->i, redo->j, redo->s);
}