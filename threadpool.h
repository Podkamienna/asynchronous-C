#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "synchronized_queue.h"

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

typedef struct runnable {
    void (*function)(void *, size_t);

    void *arg;
    size_t argsz;
} runnable_t;

typedef struct thread_pool {
    size_t number_of_working;
    bool is_destroyed; // flag that is set to true when the thread_pool becomes destroyed
    pthread_cond_t empty;
    pthread_mutex_t lock;
    pthread_t *threads; // working threads array
    queue_t *waiting; // queue of runnables waiting for execution
} thread_pool_t;

int thread_pool_init(thread_pool_t *pool, size_t pool_size);

void thread_pool_destroy(thread_pool_t *pool);

int defer(thread_pool_t *pool, runnable_t runnable);

#endif
