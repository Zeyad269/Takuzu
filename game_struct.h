/**
 * @file game_struct.h
 * @brief Definition of the structure game_s.
 **/

#include "game.h"
#include "queue.h"

struct game_s {
    square **tab;
    queue undo_redo;
    uint length;
    uint width;
    bool uniqueOn;
    bool wrappingOn;
};

struct move_m {
    int i;
    int j;
    square s;
};

struct queue_s {
    struct element_s *head;
    struct element_s *tail;
    unsigned int length;
};

struct element_s {
    move_m data;
    struct element_s *next;
    struct element_s *prev;
};
