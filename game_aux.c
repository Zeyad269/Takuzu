#include "game_aux.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"

void game_print(cgame g) {
    printf("   ");
    for (int i = 0; i < game_nb_cols(g); i++) {
        printf("%d", i);
    }
    printf("\n   ");
    for (int i = 0; i < game_nb_cols(g); i++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < game_nb_rows(g); i++) {
        // We display the number of each line
        printf("%d |", i);
        for (int j = 0; j < game_nb_cols(g); j++) {
            // We set in each case the letter corresponding to the
            // square of the case
            square s = game_get_square(g, i, j);
            if (s == S_IMMUTABLE_ZERO)
                printf("W");
            else if (s == S_IMMUTABLE_ONE)
                printf("B");
            else if (s == S_ZERO)
                printf("w");
            else if (s == S_ONE)
                printf("b");
            else
                printf(" ");
        }
        printf("|\n");
    }
    printf("   ");
    for (int i = 0; i < game_nb_cols(g); i++) {
        printf("-");
    }
    printf("\n");
}

game game_default(void) {
    game g = game_new_empty();
    game_set_square(g, 0, 1, S_IMMUTABLE_ONE);
    game_set_square(g, 0, 2, S_IMMUTABLE_ZERO);
    game_set_square(g, 2, 1, S_IMMUTABLE_ZERO);
    game_set_square(g, 2, 4, S_IMMUTABLE_ZERO);
    game_set_square(g, 3, 1, S_IMMUTABLE_ZERO);
    game_set_square(g, 3, 2, S_IMMUTABLE_ONE);
    game_set_square(g, 4, 2, S_IMMUTABLE_ONE);
    game_set_square(g, 4, 5, S_IMMUTABLE_ZERO);
    game_set_square(g, 5, 5, S_IMMUTABLE_ZERO);
    return g;
}

game game_default_solution(void) {
    game g = game_default();
    game_set_square(g, 0, 0, S_ZERO);
    game_set_square(g, 0, 3, S_ONE);
    game_set_square(g, 0, 4, S_ZERO);
    game_set_square(g, 0, 5, S_ONE);
    game_set_square(g, 1, 0, S_ZERO);
    game_set_square(g, 1, 1, S_ONE);
    game_set_square(g, 1, 2, S_ONE);
    game_set_square(g, 1, 3, S_ZERO);
    game_set_square(g, 1, 4, S_ONE);
    game_set_square(g, 1, 5, S_ZERO);
    game_set_square(g, 2, 0, S_ONE);
    game_set_square(g, 2, 2, S_ZERO);
    game_set_square(g, 2, 3, S_ONE);
    game_set_square(g, 2, 5, S_ONE);
    game_set_square(g, 3, 0, S_ONE);
    game_set_square(g, 3, 3, S_ZERO);
    game_set_square(g, 3, 4, S_ZERO);
    game_set_square(g, 3, 5, S_ONE);
    game_set_square(g, 4, 0, S_ZERO);
    game_set_square(g, 4, 1, S_ONE);
    game_set_square(g, 4, 3, S_ZERO);
    game_set_square(g, 4, 4, S_ONE);
    game_set_square(g, 5, 0, S_ONE);
    game_set_square(g, 5, 1, S_ZERO);
    game_set_square(g, 5, 2, S_ZERO);
    game_set_square(g, 5, 3, S_ONE);
    game_set_square(g, 5, 4, S_ONE);
    return g;
}
