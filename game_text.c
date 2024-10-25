#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

#define DEFAULT_SIZE 6

void list_error(game g, uint i, uint j) {
    for (i = 0; i < game_nb_rows(g); i++) {
        for (j = 0; j < game_nb_cols(g); j++) {
            if (game_has_error(g, i, j) != 0) {
                printf("The square (%u,%u) is not valide\n", i, j);
            }
        }
    }
}

void help() {
    printf("> action: help\n");
    printf("- press 'w <i> <j>' to put a zero/white at square (i,j)\n");
    printf("- press 'b <i> <j>' to put a one/black at square (i,j)\n");
    printf("- press 'e <i> <j> to put an empty square (i,j)\n");
    printf("- press 'r' to restart\n");
    printf("- press 'q' to quit\n");
}

void restart(game g) {
    printf("> action: restart\n");
    game_restart(g);
}

void quit() {
    printf("> action: quit\n");
    printf("Shame on you :-(\n");
}

void play(game g, uint i, uint j, square s, char c) {
    game_play_move(g, i, j, s);
    printf("> action: play move '%c' into square (%u,%u)\n", c, i, j);
}

int main(int argc, char* argv[]) {
    game g = game_default();

    if (argc == 2) {
        g = game_load(argv[1]);
    }

    while (game_is_over(g) == 0) {
        game_print(g);
        int temp;
        uint i, j;
        list_error(g, i, j);

        printf("> ? [h for help]\n");
        char c1;
        temp = scanf(" %c", &c1);
        if (c1 == 'h') {
            help();
        } else if (c1 == 'r') {
            restart(g);
        } else if (c1 == 'q') {
            quit();
            return EXIT_SUCCESS;
        } else if (c1 == 'w' || c1 == 'b' || c1 == 'e') {
            temp = scanf("%u %u", &i, &j);
            if (c1 == 'w' && game_check_move(g, i, j, S_ZERO) != false) {
                play(g, i, j, S_ZERO, c1);
            } else if (c1 == 'b' && game_check_move(g, i, j, S_ONE) != false) {
                play(g, i, j, S_ONE, c1);
            } else if (c1 == 'e' &&
                       game_check_move(g, i, j, S_EMPTY) != false) {
                play(g, i, j, S_EMPTY, c1);
            }
        } else if (c1 == 's') {
            char name;
            temp = scanf("%s", &name);
            temp = temp;
            game_save(g, &name);
        }
    }
    game_print(g);
    printf("Congratulation :)\n");
    return EXIT_SUCCESS;
}
