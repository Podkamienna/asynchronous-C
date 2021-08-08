#ifndef ASYNC_SYNCHRONIZED_QUEUE_H
#define ASYNC_SYNCHRONIZED_QUEUE_H

#include <stdbool.h>
#include <pthread.h>

typedef struct runnable runnable_t;

//Invariant - both value and next not NULL
typedef struct queue_node {
    void *value;
    struct queue_node *next;
} queue_node_t;

//Invariant - if first is not NULL then last not NULL
typedef struct queue {
    pthread_mutex_t mutex;
    queue_node_t *first;
    queue_node_t *last;
} queue_t;

/**
 * @brief Checks if the queue is empty.
 * @param queue — queue to be checked
 * @return Value @p true if the given pointer is NULL or the queue is empty,
 * @p false otherwise.
 */
bool empty_queue(queue_t *queue);

/**
 * @brief Initializes new queue.
 * @return The pointer to new object, NULL if it
 * was not possible to allocate memory or mutex
 * initialization failed.
 */
queue_t *init_queue();

/**
 * @brief Inserts value to the end of the queue.
 * @param queue — queue to be inserted
 * @param value — value to insert
 * @return Returns value @p -1 if mutex lock failed, given queue is NULL,
 * or couldn't allocate memory.
 */
int push_queue(queue_t *queue, void *value);

/**
 * @brief Returns the value from the top of the queue.
 * @param queue — queue from which the value is to be returned.
 * @return Value from the top of the queue.
 */
void *top_queue(queue_t *queue);

/**
 * @brief Deletes the element from the top of the queue.
 * @param queue — queue from which element is to be deleted
 */
void pop_queue(queue_t *queue);

/**
 * @brief Destroys the given queue.
 * @param queue — queue to be destroyed
 */
void destroy_queue(queue_t *queue);

#endif //ASYNC_SYNCHRONIZED_QUEUE_H
