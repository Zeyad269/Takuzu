#include <assert.h>
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

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("1");
        return EXIT_FAILURE;
    }
    if (argc > 4) {
        printf("2");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-s") == 0) {
        game g = game_load(argv[2]);
        game_solve(g);
        game_save(g, argv[3]);
    }

    else if (strcmp(argv[1], "-c") == 0) {
        game g = game_load(argv[2]);
        int TotalSolutions = game_nb_solutions(g);
        FILE* f = fopen(argv[3], "w");
        fprintf(f, "%u\n", TotalSolutions);
        fclose(f);
    }

    else
        exit(EXIT_FAILURE);
    return EXIT_SUCCESS;
}