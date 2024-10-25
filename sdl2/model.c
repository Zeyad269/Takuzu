// SDL2 Demo by aurelien.esnard@u-bordeaux.fr
#include "model.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

#define FONT "arial.ttf"
#define FONTSIZE 42
#define BACKGROUND "res/background.jpg"
#define ASTEROIDE "res/asteroide.png"
#define ASTEROIDE2 "res/asteroide2.png"
#define ETOILE "res/etoile-filante.png"
#define ETOILE2 "res/etoile-filante2.png"
#define HELP "res/question.png"
#define SOLUTION "res/solution.png"
#define UNDO "res/undo.png"
#define REDO "res/redo.png"
#define RESET "res/reset.png"
#define SAVE "res/save.png"
#define TITLE "res/Takuzu.png"
#define VICTORY "res/victory.png"



/* **************************************************************** */

struct Env_t {
  SDL_Texture *background;
  SDL_Texture *asteroide;
  SDL_Texture *immutable_asteroide;
  SDL_Texture *etoile;
  SDL_Texture *immutable_etoile;
  SDL_Texture *title;
  SDL_Texture *help;
  SDL_Texture *solver;
  SDL_Texture *undo;
  SDL_Texture *redo;
  SDL_Texture *reset;
  SDL_Texture *save;
  SDL_Texture *victory;
  SDL_Rect **tabOfRect;
  SDL_Rect rect;
  SDL_Rect helprect;
  SDL_Rect solverect;
  SDL_Rect undorect;
  SDL_Rect redorect;
  SDL_Rect resetrect;
  SDL_Rect saverect;
  game g;
  int grid_x1, grid_x2, grid_y1, grid_y2;
  int case_width, case_height;
  int inital_x, inital_y;
  int width, height;
  int w, h;
  bool isOver;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  env->isOver =false;

  /* creation of the game*/
  if (argc >= 2) {
    env->g = game_load(argv[1]);
  } else {
    env->g = game_default();
  }

  /* get current window size */
  SDL_GetWindowSize(win, &env->w, &env->h);

  /* get current grid dimensions */
  env->width = env->g->width;
  env->height = env->g->length;

  /* allocate memory of tabOfRect */
  env->tabOfRect = malloc((env->width) * sizeof(SDL_Rect *));
  for (int i = 0; i < env->width; i++) {
    env->tabOfRect[i] = malloc((env->height) * sizeof(SDL_Rect));
  }

  /* init background texture from PNG image */
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  /* init asteroide texture from PNG image */
  env->asteroide = IMG_LoadTexture(ren, ASTEROIDE);
  if (!env->asteroide) ERROR("IMG_LoadTexture: %s\n", ASTEROIDE);

  /* init immutable asteroide texture from PNG image */
  env->immutable_asteroide = IMG_LoadTexture(ren, ASTEROIDE2);
  if (!env->immutable_asteroide) ERROR("IMG_LoadTexture: %s\n", ASTEROIDE2);

  /* init etoile texture from PNG image */
  env->etoile = IMG_LoadTexture(ren, ETOILE);
  if (!env->etoile) ERROR("IMG_LoadTexture: %s\n", ETOILE);

  /* init immutable etoile texture from PNG image */
  env->immutable_etoile = IMG_LoadTexture(ren, ETOILE2);
  if (!env->immutable_etoile) ERROR("IMG_LoadTexture: %s\n", ETOILE2);

  /* init help texture from PNG image */
  env->help = IMG_LoadTexture(ren, HELP);
  if (!env->help) ERROR("IMG_LoadTexture: %s\n", HELP);

  /* init solver texture from PNG image */
  env->solver = IMG_LoadTexture(ren, SOLUTION);
  if (!env->solver) ERROR("IMG_LoadTexture: %s\n", SOLUTION);

  /* init undo texture from PNG image */
  env->undo = IMG_LoadTexture(ren, UNDO);
  if (!env->undo) ERROR("IMG_LoadTexture: %s\n", UNDO);

  /* init redo texture from PNG image */
  env->redo = IMG_LoadTexture(ren, REDO);
  if (!env->redo) ERROR("IMG_LoadTexture: %s\n", REDO);

  /* init reset texture from PNG image */
  env->reset = IMG_LoadTexture(ren, RESET);
  if (!env->reset) ERROR("IMG_LoadTexture: %s\n", RESET);

  /* init save texture from PNG image */
  env->save = IMG_LoadTexture(ren, SAVE);
  if (!env->save) ERROR("IMG_LoadTexture: %s\n", SAVE);

  /* init title texture from PNG image */
  env->title = IMG_LoadTexture(ren, TITLE);
  if (!env->title) ERROR("IMG_LoadTexture: %s\n", TITLE);

  /* init victory texture from PNG image */
  env->victory = IMG_LoadTexture(ren, VICTORY);
  if (!env->victory) ERROR("IMG_LoadTexture: %s\n", VICTORY);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* get current window size */
  SDL_GetWindowSize(win, &env->w, &env->h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  /* boundaries of the grid */
  env->grid_x1 = env->w / 5;
  env->grid_x2 = env->w - env->grid_x1 / 4;
  env->grid_y1 = env->h / 5;
  env->grid_y2 = env->h - (env->grid_y1 / 4);

  /* set the color of the grid */
  env->rect.w = env->case_width * env->width;
  env->rect.h = env->case_height * env->height;
  env->rect.x = env->grid_x1;
  env->rect.y = env->grid_y1;
  SDL_SetRenderDrawColor(ren, 0xA0, 0xA0, 0xA0, 0xFF); /* Grey */
  SDL_RenderFillRect(ren, &env->rect);
  SDL_RenderCopy(ren, NULL, NULL, &env->rect);

  /* dimensions of a case */
  env->case_width = (env->grid_x2 - env->grid_x1) / (env->width);
  env->case_height = (env->grid_y2 - env->grid_y1) / (env->height);

  /* inital position of the first rect */
  env->inital_x = env->grid_x1 + (env->case_width) / 5;
  env->inital_y = env->grid_y1 + (env->case_height) / 5;

  /* render rects textures */
  for (int i = 0; i < env->height; i++) {
    for (int j = 0; j < env->width; j++) {
      /* dimensions */
      (env->tabOfRect[i][j]).w = env->case_width / 1.5;
      (env->tabOfRect[i][j]).h = env->case_height / 1.5;

      /* positions */
      (env->tabOfRect[i][j]).x = env->inital_x + j * env->case_width;
      (env->tabOfRect[i][j]).y = env->inital_y + i * env->case_height;
    }
  }

  /* render a grid with lines */
  SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE); /* white */

  /* draw vertical lines */
  for (int i = env->grid_x1; i <= env->grid_x2; i += env->case_width) {
    SDL_RenderDrawLine(ren, i, env->grid_y1, i, env->grid_y2);
  }
  /* draw horizontal lines */
  for (int i = env->grid_y1; i <= env->grid_y2; i += env->case_height) {
    SDL_RenderDrawLine(ren, env->grid_x1, i, env->grid_x2, i);
  }

  /* Render the grid */
  if(game_is_over(env->g)){
    env->isOver = true;
  }

  for (int i = 0; i < env->width; i++) {
    for (int j = 0; j < env->height; j++) {
      if (game_has_error(env->g, i, j) != 0) {
        SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF); /* Red */
        SDL_RenderFillRect(ren, &(env->tabOfRect[i][j]));
        SDL_RenderCopy(ren, NULL, NULL, &(env->tabOfRect[i][j]));
      }
      if (game_get_square(env->g, i, j) == S_ZERO) {
        SDL_RenderCopy(ren, env->asteroide, NULL, &(env->tabOfRect[i][j]));
      } else if (game_get_square(env->g, i, j) == S_IMMUTABLE_ZERO) {
        SDL_RenderCopy(ren, env->immutable_asteroide, NULL, &(env->tabOfRect[i][j]));
      } else if (game_get_square(env->g, i, j) == S_IMMUTABLE_ONE) {
        SDL_RenderCopy(ren, env->immutable_etoile, NULL, &(env->tabOfRect[i][j]));
      } else if (game_get_square(env->g, i, j) == S_ONE) {
        SDL_RenderCopy(ren, env->etoile, NULL, &(env->tabOfRect[i][j]));
      } else if (game_get_square(env->g, i, j) == S_EMPTY) {
        SDL_SetRenderDrawColor(ren, 0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderFillRect(ren, &(env->tabOfRect[i][j]));
        SDL_RenderCopy(ren, NULL, NULL, &(env->tabOfRect[i][j]));
      }
    }
  }

  /* render text texture */
  env->rect.w = (env->grid_x2 - env->grid_x1) / 2;
  env->rect.h = env->grid_y1 * 1.5;
  env->rect.x = (env->w / 2) - (env->rect.w / 2);
  env->rect.y = (env->grid_y1 / 2) - (env->rect.h / 2);
  SDL_RenderCopy(ren, env->title, NULL, &env->rect);

  /* render options texture */

  /*Dimensions*/
  env->helprect.w = env->grid_x1 / 2.5;
  env->helprect.h = (env->grid_y2 - env->grid_y1) / 8;
  env->helprect.x = env->grid_x1 / 3;

  env->undorect.w = env->grid_x1 / 2.5;
  env->undorect.h = (env->grid_y2 - env->grid_y1) / 8;
  env->undorect.x = env->grid_x1 / 3;

  env->redorect.w = env->grid_x1 / 2.5;
  env->redorect.h = (env->grid_y2 - env->grid_y1) / 8;
  env->redorect.x = env->grid_x1 / 3;

  env->resetrect.w = env->grid_x1 / 2.5;
  env->resetrect.h = (env->grid_y2 - env->grid_y1) / 8;
  env->resetrect.x = env->grid_x1 / 3;

  env->saverect.w = env->grid_x1 / 2.5;
  env->saverect.h = (env->grid_y2 - env->grid_y1) / 8;
  env->saverect.x = env->grid_x1 / 3;

  env->solverect.w = env->grid_x1 / 2.5;
  env->solverect.h = (env->grid_y2 - env->grid_y1) / 8;
  env->solverect.x = env->grid_x1 / 3;


  /*Positions*/

  /*help*/
  env->helprect.y = env->inital_y / 3.5;
  SDL_RenderCopy(ren, env->help, NULL, &env->helprect);
  /*solver*/
  env->solverect.y = env->inital_y;
  SDL_RenderCopy(ren, env->solver, NULL, &env->solverect);
  /*undo*/
  env->undorect.y = env->inital_y + 1.5 * env->redorect.h;
  SDL_RenderCopy(ren, env->undo, NULL, &env->undorect);
  /*redo*/
  env->redorect.y = env->undorect.y + 1.5 * env->redorect.h;
  SDL_RenderCopy(ren, env->redo, NULL, &env->redorect);
  /*reset*/
  env->resetrect.y = env->redorect.y + 1.5 * env->resetrect.h;
  SDL_RenderCopy(ren, env->reset, NULL, &env->resetrect);
  /*save*/
  env->saverect.y = env->resetrect.y + 1.5 * env->saverect.h;
  SDL_RenderCopy(ren, env->save, NULL, &env->saverect);

  /* Render victory texture */
  if(env->isOver){
    SDL_RenderCopy(ren, env->victory, NULL, NULL);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  SDL_GetWindowSize(win, &env->w, &env->h);

  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */
  if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    int x = -1, y = -1;

    /*Tests if the mouse is inside the grid */
    if ((mouse.x > env->grid_x1 && mouse.x < env->grid_x2) && (mouse.y > env->grid_y1 && mouse.y < env->grid_y2)) {
      /*Get the the x position of the mouse in the grid*/
      for (int i = 0; i <= (mouse.x - env->grid_x1); i += env->case_width) {
        x++;
      }
      /*Get the the y position of the mouse in the grid*/
      for (int i = 0; i <= (mouse.y - env->grid_y1); i += env->case_height) {
        y++;
      }

      /* x and y are reversed because in Render -> tabOfRect, width eand height are reversed aswell
      The left click button of the mouse is being clicked */
      if (e->button.button == SDL_BUTTON_LEFT) {
        if (game_get_square(env->g, y, x) == S_EMPTY) {
          game_set_square(env->g, y, x, S_ONE);
        } else if (game_get_square(env->g, y, x) == S_ZERO) {
          game_set_square(env->g, y, x, S_EMPTY);
        } else if (game_get_square(env->g, y, x) == S_ONE) {
          game_set_square(env->g, y, x, S_ZERO);
        }
      }  // The left click button of the mouse is being clicked
      else if (e->button.button == SDL_BUTTON_RIGHT) {
        if (game_get_square(env->g, y, x) == S_EMPTY) {
          game_set_square(env->g, y, x, S_ZERO);
        } else if (game_get_square(env->g, y, x) == S_ZERO) {
          game_set_square(env->g, y, x, S_ONE);
        } else if (game_get_square(env->g, y, x) == S_ONE) {
          game_set_square(env->g, y, x, S_EMPTY);
        }
      }
    }

    /*Tests if the mouse is upon the help option button */
    if ((mouse.x > env->helprect.x && mouse.x < env->helprect.x + env->helprect.w) &&
        (mouse.y > env->helprect.y && mouse.y < env->helprect.y + env->helprect.h)) {
      // Sprite a new window
      // Add this sentence at the bottom of the window :
      // printf("There is %u possibles solutions for this game", game_nb_solutions(env->g));
    }

    /*Tests if the mouse is upon the solution option button */
    if ((mouse.x > env->solverect.x && mouse.x < env->solverect.x + env->solverect.w) &&
        (mouse.y > env->solverect.y && mouse.y < env->solverect.y + env->solverect.h)) {
      if (game_equal(env->g, game_default())) {
        env->g = game_default_solution();
      } else
        game_solve(env->g);
    }
    /*Tests if the mouse is upon the undo option button */
    if ((mouse.x > env->undorect.x && mouse.x < env->undorect.x + env->undorect.w) &&
        (mouse.y > env->undorect.y && mouse.y < env->undorect.y + env->undorect.h)) {
      game_undo(env->g);
    }
    /*Tests if the mouse is upon the redo option button */
    else if ((mouse.x > env->redorect.x && mouse.x < env->redorect.x + env->redorect.w) &&
             (mouse.y > env->redorect.y && mouse.y < env->redorect.y + env->redorect.h)) {
      game_redo(env->g);
    }
    /*Tests if the mouse is upon the reset option button */
    else if ((mouse.x > env->resetrect.x && mouse.x < env->resetrect.x + env->resetrect.w) &&
             (mouse.y > env->resetrect.y && mouse.y < env->resetrect.y + env->resetrect.h)) {
      game_restart(env->g);
    }
    /*Tests if the mouse is upon the save option button */
    else if ((mouse.x > env->saverect.x && mouse.x < env->saverect.x + env->saverect.w) &&
             (mouse.y > env->saverect.y && mouse.y < env->saverect.y + env->saverect.h)) {
      game_save(env->g, "save.txt");
    }
  }
  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* Clean Memory */
  SDL_DestroyTexture(env->title);
  SDL_DestroyTexture(env->asteroide);
  SDL_DestroyTexture(env->immutable_asteroide);
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->etoile);
  SDL_DestroyTexture(env->immutable_asteroide);
  SDL_DestroyTexture(env->help);
  SDL_DestroyTexture(env->solver);
  SDL_DestroyTexture(env->undo);
  SDL_DestroyTexture(env->redo);
  SDL_DestroyTexture(env->reset);
  SDL_DestroyTexture(env->save);
  for (int i = 0; i < env->width; i++) {
    free(env->tabOfRect[i]);
  }
  free(env->tabOfRect);
  free(env);
}

/* **************************************************************** */
