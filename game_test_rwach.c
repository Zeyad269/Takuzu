#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

bool test_dummy() { return true; }

bool test_game_get_square() {
    // we create an empty game
    game g = game_new_empty();
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            // we fill the game with S_ZERO squares
            game_set_square(g, i, j, S_ZERO);
            // we verify that the game_get_square() return the state we set
            if (game_get_square(g, i, j) != S_ZERO) {
                game_delete(g);
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

bool test_game_get_number() {
    // we create an empty game
    game g = game_new_empty();
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        // we fill column 0 with S_IMMUTABLE_ZERO that got 0 as number
        game_set_square(g, i, 0, S_IMMUTABLE_ZERO);
        // we fill column 1 with S_IMMUTABLE_ONE that got 1 as number
        game_set_square(g, i, 1, S_IMMUTABLE_ONE);
        // we fill column 2 with S_ZERO that got 0 as number
        game_set_square(g, i, 2, S_ZERO);
        // we fill column 3 with S_ONE that got 1 as number
        game_set_square(g, i, 3, S_ONE);
        // we verify that every square got the right number
        if (game_get_number(g, i, 0) != 0 || game_get_number(g, i, 1) != 1 ||
            game_get_number(g, i, 2) != 0 || game_get_number(g, i, 3) != 1 ||
            game_get_number(g, i, 4) != -1) {
            game_delete(g);
            return false;
        }
    }
    game_delete(g);
    return true;
}

bool test_game_get_next_square() {
    // we create an empty game
    game g = game_new_empty();
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            // we fill the game with S_ZERO squares
            game_set_square(g, i, j, S_ZERO);
        }
    }
    // we put a square with a different value than S_ZERO to verify that the
    // function don't always return S_ZERO
    game_set_square(g, 3, 4, S_ONE);

    if (game_get_next_square(g, 3, 3, UP, 1) != S_ZERO ||
        game_get_next_square(g, 3, 3, DOWN, 1) != S_ZERO ||
        game_get_next_square(g, 3, 3, RIGHT, 1) != S_ONE ||
        game_get_next_square(g, 3, 3, LEFT, 1) != S_ZERO ||
        game_get_next_square(g, 5, 5, RIGHT, 1) != -1) {
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

bool test_game_is_empty() {
    // we create an empty game
    game g = game_new_empty();
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        // we fill column 0 with S_IMMUTABLE_ZERO that got 0 as number
        game_set_square(g, i, 0, S_IMMUTABLE_ZERO);
        // we fill column 1 with S_IMMUTABLE_ONE that got 1 as number
        game_set_square(g, i, 1, S_IMMUTABLE_ONE);
        // we fill column 2 with S_ZERO that got 0 as number
        game_set_square(g, i, 2, S_ZERO);
        // we fill column 3 with S_ONE that got 1 as number
        game_set_square(g, i, 3, S_ONE);
        // we fill column 4 with S_ONE that got 1 as number
        game_set_square(g, i, 4, S_ONE);
        // we fill column 5 with S_ONE that got 1 as number
        game_set_square(g, i, 5, S_ONE);
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            // we verify that none of the squares are empty
            if (game_is_empty(g, i, j) != false) return false;
        }
    }
    game_delete(g);
    return true;
}

bool test_game_get_next_number() {
    // we create an empty game
    game g = game_new_empty();
    // we fill the game with S_ZERO squares
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            game_set_square(g, i, j, S_ZERO);
        }
    }
    // we put a square with a different value than S_ZERO to verify that the
    // function do not always return S_ZERO
    game_set_square(g, 3, 4, S_ONE);

    if (game_get_next_number(g, 3, 3, UP, 1) != 0 ||
        game_get_next_number(g, 3, 3, DOWN, 1) != 0 ||
        game_get_next_number(g, 3, 3, RIGHT, 1) != 1 ||
        game_get_next_number(g, 3, 3, LEFT, 1) != 0 ||
        game_get_next_number(g, 5, 5, DOWN, 1) != -1 ||
        game_get_next_number(g, 0, 0, LEFT, 1) != -1) {
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

bool test_game_is_immutable() {
    // we create an empty game
    game g = game_new_empty();
    // we fill half of the game with S_IMMUTABLE_ZERO squares
    for (int i = 0; i < DEFAULT_SIZE / 2; i++) {
        for (int j = 0; j < DEFAULT_SIZE / 2; j++) {
            game_set_square(g, i, j, S_IMMUTABLE_ZERO);
            if (game_is_immutable(g, i, j) == false) {
                game_delete(g);
                return false;
            }
        }
    }
    // we fill the other half of the game with S_IMMUTABLE_ONE squares
    for (int i = DEFAULT_SIZE / 2; i < DEFAULT_SIZE; i++) {
        for (int j = DEFAULT_SIZE / 2; j < DEFAULT_SIZE; j++) {
            game_set_square(g, i, j, S_IMMUTABLE_ONE);
            if (game_is_immutable(g, i, j) == false) {
                game_delete(g);
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

bool test_game_print() {
    // we create an empty game
    game g = game_new_empty();
    // we verify that the game_print() function doesn't occurred a fatal error
    game_print(g);
    game_delete(g);
    return true;
}

bool test_game_get_next_number_wrappingOn() {
    // we create an empty game
    game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true, false);
    // we fill the game with S_ZERO squares
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        for (int j = 0; j < DEFAULT_SIZE; j++) {
            game_set_square(g, i, j, S_ZERO);
        }
    }
    // we put a square with a different value than S_ZERO to verify that the
    // function don't always return S_ZERO
    game_set_square(g, 3, 0, S_ONE);

    if (game_get_next_number(g, 3, 5, UP, 1) != 0 ||
        game_get_next_number(g, 3, 5, DOWN, 1) != 0 ||
        game_get_next_number(g, 3, 5, RIGHT, 1) != 1 ||
        game_get_next_number(g, 3, 5, LEFT, 1) != 0 ||
        game_get_next_number(g, 3, 3, UP, 1) != 0 ||
        game_get_next_number(g, 3, 3, DOWN, 1) != 0 ||
        game_get_next_number(g, 3, 3, RIGHT, 1) != 0 ||
        game_get_next_number(g, 3, 3, LEFT, 1) != 0) {
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

bool test_game_is_wrapping() {
    game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true, false);
    if (game_is_wrapping(g) != true) return false;
    game g2 = game_new_empty();
    if (game_is_wrapping(g2) != false) return false;
    return true;
}

bool test_game_is_unique() {
    game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, true);
    if (game_is_unique(g) != true) return false;
    game g2 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, true, false);
    if (game_is_unique(g2) != false) return false;
    game g3 = game_new_empty();
    if (game_is_unique(g3) != false) return false;
    return true;
}

bool test_game_is_over_uniqueOn() {
    game g = game_new_empty_ext(4, 4, false, true);
    // test if the function return true with a correct game
    game_set_square(g, 0, 0, S_ZERO);
    game_set_square(g, 0, 1, S_ZERO);
    game_set_square(g, 0, 2, S_ONE);
    game_set_square(g, 0, 3, S_ONE);
    game_set_square(g, 1, 0, S_ZERO);
    game_set_square(g, 1, 1, S_ONE);
    game_set_square(g, 1, 2, S_ZERO);
    game_set_square(g, 1, 3, S_ONE);
    game_set_square(g, 2, 0, S_ONE);
    game_set_square(g, 2, 1, S_ZERO);
    game_set_square(g, 2, 2, S_ONE);
    game_set_square(g, 2, 3, S_ZERO);
    game_set_square(g, 3, 0, S_ONE);
    game_set_square(g, 3, 1, S_ONE);
    game_set_square(g, 3, 2, S_ZERO);
    game_set_square(g, 3, 3, S_ZERO);
    if (game_is_over(g) != true) return false;

    // test if the function return false with a game with duplicates lines
    game g2 = game_new_empty_ext(4, 2, false, true);
    game_set_square(g2, 0, 0, S_ZERO);
    game_set_square(g2, 0, 1, S_ONE);
    game_set_square(g2, 1, 0, S_ONE);
    game_set_square(g2, 1, 1, S_ZERO);
    game_set_square(g2, 2, 0, S_ZERO);
    game_set_square(g2, 2, 1, S_ONE);
    game_set_square(g2, 3, 0, S_ONE);
    game_set_square(g2, 3, 1, S_ZERO);
    if (game_is_over(g2) != false) return false;

    // test if the function return false with a game with duplicates columns
    game g3 = game_new_empty_ext(2, 4, false, true);
    game_set_square(g3, 0, 0, S_ZERO);
    game_set_square(g3, 0, 1, S_ONE);
    game_set_square(g3, 0, 2, S_ZERO);
    game_set_square(g3, 0, 3, S_ONE);
    game_set_square(g3, 1, 0, S_ONE);
    game_set_square(g3, 1, 1, S_ZERO);
    game_set_square(g3, 1, 2, S_ONE);
    game_set_square(g3, 1, 3, S_ZERO);
    if (game_is_over(g3) != false) return false;

    return true;
}

bool test_game_save() {
    // Saving in a file a default game 6x6
    game g = game_default();
    game_save(g, "test_save");
    FILE* f = fopen("test_save", "r");
    // Value of 0 in ASCII
    uint ASCIIValue = 48;
    // Testing if the parameters are right
    // First, testing if the size parameters are rights
    uint ValuesToTest[] = {g->length, g->width};
    for (int i = 0; i < 2;) {
        int testedValue = fgetc(f);
        if (testedValue >= ASCIIValue && testedValue <= ASCIIValue + 9) {
            if (testedValue != ASCIIValue + ValuesToTest[i]) {
                fclose(f);
                return false;
            }
            i++;
        }
    }
    // Secondly, testing if the boolean parameters are rights
    bool BooleanToTest[] = {g->wrappingOn, g->uniqueOn};
    for (int i = 0; i < 2;) {
        int testedValue = fgetc(f);
        if (testedValue >= ASCIIValue && testedValue <= ASCIIValue + 9) {
            if ((BooleanToTest[i] == true && testedValue != 1 + ASCIIValue) ||
                (BooleanToTest[i] == false && testedValue != ASCIIValue)) {
                fclose(f);
                return false;
            }
            i++;
        }
    }
    // Then, testing if the squares states are rights
    for (int i = 0; i < g->length; i++) {
        for (int j = 0; j < g->width;) {
            int Value = fgetc(f);
            if (Value == 'W') {
                if (g->tab[i][j] != S_IMMUTABLE_ZERO) {
                    fclose(f);
                    return false;
                }
                j++;
            } else if (Value == 'B') {
                if (g->tab[i][j] != S_IMMUTABLE_ONE) {
                    fclose(f);
                    return false;
                }
                j++;
            } else if (Value == 'w') {
                if (g->tab[i][j] != S_ZERO) {
                    fclose(f);
                    return false;
                }
                j++;
            } else if (Value == 'b') {
                if (g->tab[i][j] != S_ONE) {
                    fclose(f);
                    return false;
                }
                j++;
            } else if (Value == 'e') {
                if (g->tab[i][j] != S_EMPTY) {
                    fclose(f);
                    return false;
                }
                j++;
            }
        }
    }

    fclose(f);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        bool ok = false;
        if (strcmp("dummy", argv[1]) == 0)
            ok = test_dummy();
        else if (strcmp("get_square", argv[1]) == 0)
            ok = test_game_get_square();
        else if (strcmp("get_number", argv[1]) == 0)
            ok = test_game_get_number();
        else if (strcmp("get_next_square", argv[1]) == 0)
            ok = test_game_get_next_square();
        else if (strcmp("is_empty", argv[1]) == 0)
            ok = test_game_is_empty();
        else if (strcmp("get_next_number", argv[1]) == 0)
            ok = test_game_get_next_number();
        else if (strcmp("is_immutable", argv[1]) == 0)
            ok = test_game_is_immutable();
        else if (strcmp("game_print", argv[1]) == 0)
            ok = test_game_print();
        else if (strcmp("get_next_number_wrappingOn", argv[1]) == 0)
            ok = test_game_get_next_number_wrappingOn();
        else if (strcmp("game_is_wrapping", argv[1]) == 0)
            ok = test_game_is_wrapping();
        else if (strcmp("game_is_unique", argv[1]) == 0)
            ok = test_game_is_unique();
        else if (strcmp("game_is_over_uniqueOn", argv[1]) == 0)
            ok = test_game_is_over_uniqueOn();
        else if (strcmp("game_save", argv[1]) == 0)
            ok = test_game_save();
        if (ok) {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}