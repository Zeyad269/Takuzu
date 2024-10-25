#include "game.h"

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

//@{

typedef struct queue_s* queue;

typedef struct move_m* move_m;

/** Creates a new queue.*/
queue queue_new();

/** Creates a new move.*/
move_m create_move(int i, int j, square s);

/** Adds a new element at the head of the queue. */
void queue_push_head(queue q, move_m data);

/* Adds a new element at the tail of the queue. */
void queue_push_tail(queue q, move_m data);

/* Removes the first element of the queue and returns its data (or NULL if the
queue is empty). The returned element must be freed manually if it was
dynamically allocated.*/
move_m queue_pop_head(queue q);

/* Removes the last element of the queue and returns its data (or NULL if the
queue is empty). The returned element must be freed manually if it was
dynamically allocated. */
move_m queue_pop_tail(queue q);

/** Returns the number of elements in queue. */
int queue_length(const queue q);

/** Returns true if the queue contains only the separator. */
bool queue_is_separator(const queue q);

/** Returns true if the queue is empty. */
bool queue_is_empty(const queue q);

/** Returns the first element of the queue. */
move_m queue_peek_head(queue q);

/** Returns the last element of the queue. */
move_m queue_peek_tail(queue q);

/** Removes all the elements in queue. If queue elements contain
 * dynamically-allocated memory, they should be freed first. */
void queue_clear(queue q);

/** Convenience method, which frees all the memory used by a queue, and calls
 * the provided destroy function on each element in the queue. */
void queue_clear_full(queue q, void (*destroy)(move_m));

/** Frees the memory allocated for the queue. If queue elements contain
dynamically-allocated memory, you should either use queue_free_full() or free
them manually first. */
void queue_free(queue q);

/** Convenience method, which frees all the memory used by a queue, and calls
the provided destroy function on every element's data. */
void queue_free_full(queue q, void (*destroy)(move_m));

//@}

#endif