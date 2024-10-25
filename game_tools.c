#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

game game_load(char *filename) {
    FILE *f = fopen(filename, "r");
    // Value of 0 in ASCII
    int ASCIIValue = 48;

    // We stock the parameters in a list
    int params[4];
    for (uint i = 0; i < 4;) {
        int value = fgetc(f);
        int next_value = fgetc(f);
        // If our value is an invalid value (greater than 9) we
        // exit
        if ((value >= ASCIIValue && value <= (ASCIIValue + 9)) &&
            (next_value >= ASCIIValue && next_value <= (ASCIIValue + 9))) {
            exit(EXIT_FAILURE);
        }
        // If our value is a valid value (a number between 0 and 9 include) we
        // stock it in the list
        if ((value >= ASCIIValue && value <= (ASCIIValue + 9))) {
            params[i] = value - ASCIIValue;
            i++;
        }
    }

    // We stock the squares in a list to use it later with the game_new_ext()
    square squares[params[0] * params[1]];
    for (uint i = 0; i < (params[0] * params[1]);) {
        int value = fgetc(f);
        if (value == 'W') {
            squares[i] = S_IMMUTABLE_ZERO;
            i++;
        } else if (value == 'B') {
            squares[i] = S_IMMUTABLE_ONE;
            i++;
        } else if (value == 'w') {
            squares[i] = S_ZERO;
            i++;
        } else if (value == 'b') {
            squares[i] = S_ONE;
            i++;
        } else if (value == 'e') {
            squares[i] = S_EMPTY;
            i++;
        } else
            ;
    }
    // We create the game
    game g = game_new_ext(params[0], params[1], squares, params[2], params[3]);
    return g;
}

void game_save(cgame g, char *filename) {
    // We print in the file the number of rows and the number of columns also we
    // print the state of wrapping and unique mode (0 ->
    // false, 1 -> true )
    FILE *f = fopen(filename, "w");
    uint LengthAndWidth[] = {g->length, g->width};
    // Value of 0 in ASCII
    uint ASCIIValue = 48;

    for (uint i = 0; i < 2; i++) {
        fputc(LengthAndWidth[i] + ASCIIValue, f);
        fprintf(f, " ");
    }

    if (g->wrappingOn == false)
        fputc(ASCIIValue, f);
    else
        fputc(ASCIIValue + 1, f);
    fprintf(f, " ");

    if (g->uniqueOn == false)
        fputc(ASCIIValue, f);
    else
        fputc(ASCIIValue + 1, f);

    // We go through the grid, and depending on the state of the square we are
    // printing in the file the corresponding letter
    for (uint i = 0; i < g->length; i++) {
        fprintf(f, "\n");
        for (uint j = 0; j < g->width; j++) {
            if (g->tab[i][j] == S_ZERO)
                fputc('w', f);
            else if (g->tab[i][j] == S_ONE)
                fputc('b', f);
            else if (g->tab[i][j] == S_IMMUTABLE_ZERO)
                fputc('W', f);
            else if (g->tab[i][j] == S_IMMUTABLE_ONE)
                fputc('B', f);
            else
                fputc('e', f);
        }
    }
    fclose(f);
    return;
}

void solving(cgame g, int i, int j, uint *count, bool OnlyFindOne) {
    if (i == g->length && j == g->width - 1) {
        if (game_is_over(g)) {
            (*count)++;
        }
        // Case we want to return only one solution ( The first we find )
        if ((OnlyFindOne) && (*count == 1)) {
            return;
        }
        return;
    }

    if ((game_get_next_number(g, i, j, LEFT, 1) == g->tab[i][j] &&
         game_get_next_number(g, i, j, LEFT, 2) == g->tab[i][j]))
        return;
    if (game_get_next_number(g, i, j, DOWN, 1) == g->tab[i][j] &&
        game_get_next_number(g, i, j, DOWN, 2) == g->tab[i][j])
        return;

    if (g->tab[i][j] != S_IMMUTABLE_ONE && g->tab[i][j] != S_IMMUTABLE_ZERO) {
        g->tab[i][j] = S_ZERO;
        if (i == g->length - 1 && j != g->width - 1) {
            solving(g, 0, j + 1, count, OnlyFindOne);
            if ((OnlyFindOne) && *count == 1) return;
        } else {
            solving(g, i + 1, j, count, OnlyFindOne);
            if ((OnlyFindOne) && *count == 1) return;
        }

        g->tab[i][j] = S_ONE;
        if (i == g->length - 1 && j != g->width - 1) {
            solving(g, 0, j + 1, count, OnlyFindOne);
            if ((OnlyFindOne) && *count == 1) return;
        } else {
            solving(g, i + 1, j, count, OnlyFindOne);
            if ((OnlyFindOne) && *count == 1) return;
        }

        // If our square isn't empty, we move on to the next value
    } else if (i == g->length - 1 && j != g->width - 1) {
        solving(g, 0, j + 1, count, OnlyFindOne);
        if ((OnlyFindOne) && *count == 1) return;
    } else {
        solving(g, i + 1, j, count, OnlyFindOne);
        if ((OnlyFindOne) && *count == 1) return;
    }
}

uint game_nb_solutions(cgame g) {
    uint count = 0;
    cgame g_temp = game_copy(g);
    solving(g_temp, 0, 0, &count, false);
    return count;
}

bool game_solve(game g) {
    uint count = 0;
    solving(g, 0, 0, &count, true);
    return false;
}


/* ************************************************************************** */
/*                            Random Game Generator                           */
/* ************************************************************************** */

static game game_set_some_mutable_squares(game g, uint nb_empty)
{
  assert(g);
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  uint nb_squares = nb_rows * nb_cols;
  assert(nb_empty <= nb_squares);
  uint nb_empty1 = nb_empty;
  while (nb_empty1 > 0) {
    uint pos = rand() % nb_squares;
    uint row = pos / nb_cols;
    uint col = pos % nb_cols;
    if (game_is_immutable(g, row, col)) {
      square s = game_get_number(g, row, col);
      game_set_square(g, row, col, s + 1);
      nb_empty1--;
    }
  }
  return g;
}

/* ************************************************************************** */

static void game_flip_one_square(game g, int i, int j)
{
  square s = game_get_square(g, i, j);
  if (s == S_IMMUTABLE_ONE)
    game_set_square(g, i, j, S_IMMUTABLE_ZERO);
  else
    game_set_square(g, i, j, S_IMMUTABLE_ONE);
}

/* ************************************************************************** */

static bool try_flip(game g, int i1, int j1, int i2, int j2)
{
  if (i1 == i2 && j1 == j2) return false;
  assert(game_is_over(g));
  square s11 = game_get_square(g, i1, j1);
  square s22 = game_get_square(g, i2, j2);
  square s12 = game_get_square(g, i1, j2);
  square s21 = game_get_square(g, i2, j1);

  if (s11 == s22 && s12 == s21 && s11 != s12) {
    game_flip_one_square(g, i1, j1);
    game_flip_one_square(g, i2, j2);
    game_flip_one_square(g, i1, j2);
    game_flip_one_square(g, i2, j1);

    if (game_is_over(g)) {
      return true;
    } else {  // undo
      game_flip_one_square(g, i1, j1);
      game_flip_one_square(g, i2, j2);
      game_flip_one_square(g, i1, j2);
      game_flip_one_square(g, i2, j1);
      assert(game_is_over(g));
      return false;
    }
  }
  return false;
}

/* ************************************************************************** */

static void apply_nb_flips(game g, uint nb)
{
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  while (nb > 0) {
    uint i1 = rand() % nb_rows;
    uint j1 = rand() % nb_cols;
    uint i2 = rand() % nb_rows;
    uint j2 = rand() % nb_cols;
    if (try_flip(g, i1, j1, i2, j2)) nb--;
  }
}

/* ************************************************************************** */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint uniq, bool with_solution)
{
  // step 0: create an empty game
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, uniq);

  // fill the grid with initial values
  for (uint i = 0; i < nb_rows; i++)
    for (uint j = 0; j < nb_cols; j++) game_set_square(g, i, j, (i + j) % 2 + 3);

  // Make it unique if it is square
  if (nb_cols == nb_rows) {
    for (uint i = 0; i < nb_rows / 2; i++) {
      game_flip_one_square(g, 2 * i, 2 * i);
      game_flip_one_square(g, 2 * i, 2 * i + 1);
      game_flip_one_square(g, 2 * i + 1, 2 * i);
      game_flip_one_square(g, 2 * i + 1, 2 * i + 1);
    }
  }
  assert(game_is_over(g));
  // apply some flips
  apply_nb_flips(g, nb_rows * nb_cols * 10);

  assert(game_is_over(g));
  // check

  game_set_some_mutable_squares(g, (nb_rows * nb_cols * 1) / 2);
  if (!with_solution) game_restart(g);
  return g;
}

/* ************************************************************************** */
