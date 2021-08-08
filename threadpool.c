#include "threadpool.h"
#include "synchronized_queue.h"
#include "err.h"

#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct runnable runnable_t;
typedef struct queue queue_t;
typedef struct thread_pool thread_pool_t;

static void *runnable_function(void *thread_pool_void) {
    int err;
    thread_pool_t *thread_pool = (thread_pool_t *) thread_pool_void;

    while (true) {
        if ((err = pthread_mutex_lock(&thread_pool->lock)) != 0) {
            syserr(err, "Mutex lock failed.");
        }

        while (!thread_pool->is_destroyed && empty_queue(thread_pool->waiting)) {
            if ((err = pthread_cond_wait(&thread_pool->empty, &thread_pool->lock)) != 0) {
                syserr(err, "Condition wait failed.");
            }
        }

        if (thread_pool->is_destroyed && empty_queue(thread_pool->waiting)) {
            if ((err = pthread_mutex_unlock(&thread_pool->lock)) != 0) {
                syserr(err, "Mutex unlock failed.");
            }

            break;
        }

        runnable_t *runnable = top_queue(thread_pool->waiting);
        pop_queue(thread_pool->waiting);

        if ((err = pthread_mutex_unlock(&thread_pool->lock)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        runnable->function(runnable->arg, runnable->argsz);
        free(runnable);
    }

    return NULL;
}

int thread_pool_init(thread_pool_t *pool, size_t num_threads) {
    int err;

    if (pool == NULL) {
        return -1;
    }

    pool->number_of_working = 0;
    pool->is_destroyed = false;
    pool->threads = malloc(num_threads * sizeof(pthread_t *));
    pool->waiting = init_queue();

    if (pool->threads == NULL || pool->waiting == NULL) {
        destroy_queue(pool->waiting);
        free(pool->threads);

        return -1;
    }

    if (pthread_cond_init(&pool->empty, NULL) != 0) {
        destroy_queue(pool->waiting);
        free(pool->threads);

        return -1;
    }

    if (pthread_mutex_init(&pool->lock, NULL) != 0) {
        if ((err = pthread_cond_destroy(&pool->empty)) != 0) {
            syserr(err, "Condition destroy failed.");
        }
        destroy_queue(pool->waiting);
        free(pool->threads);

        return -1;
    }

    for (size_t i = 0; i < num_threads; i++) {
        if (pthread_create(&pool->threads[i], NULL, runnable_function, pool) != 0) {
            if ((err = pthread_mutex_lock(&pool->lock)) != 0) {
                syserr(err, "Mutex lock failed.");
            }

            pool->is_destroyed = true;

            if ((err = pthread_cond_broadcast(&pool->empty)) != 0) {
                syserr(err, "Condition broadcast failed.");
            }

            if ((err = pthread_mutex_unlock(&pool->lock)) != 0) {
                syserr(err, "Mutex unlock failed.");
            }

            for (size_t j = 0; j < pool->number_of_working; j++) {
                if ((err = pthread_join(pool->threads[j], NULL)) != 0) {
                    syserr(err, "Thread join failed.");
                }
            }

            pthread_cond_destroy(&pool->empty);
            pthread_mutex_destroy(&pool->lock);
            destroy_queue(pool->waiting);
            free(pool->threads);

            return -1;
        }

        pool->number_of_working++;
    }

    return 0;
}

void thread_pool_destroy(struct thread_pool *pool) {
    int err;

    if (pool == NULL) {
        return;
    }

    if ((err = pthread_mutex_lock(&pool->lock)) != 0) {
        syserr(err, "Mutex lock failed.");
    }

    pool->is_destroyed = true;

    if ((err = pthread_cond_broadcast(&pool->empty)) != 0) {
        syserr(err, "Condition broadcast failed.");
    }

    if ((err = pthread_mutex_unlock(&pool->lock)) != 0) {
        syserr(err, "Mutex unlock failed.");
    }

    for (size_t j = 0; j < pool->number_of_working; j++) {
        if ((err = pthread_join(pool->threads[j], NULL)) != 0) {
            syserr(err, "Thread join failed");
        }
    }

    destroy_queue(pool->waiting);
    free(pool->threads);

    if ((err = pthread_cond_destroy(&pool->empty)) != 0) {
        syserr(err, "Condition destroy failed.");
    }

    if ((err = pthread_mutex_destroy(&pool->lock)) != 0) {
        syserr(err, "Mutex destroy failed.");
    }
}

int defer(struct thread_pool *pool, runnable_t runnable) {
    int err;

    if (pool == NULL) {
        return -1;
    }

    if (pool->is_destroyed) {
        return -1;
    }

    runnable_t *runnable_copy = malloc(sizeof(runnable_t));
    if (runnable_copy == NULL) {
        return -1;
    }

    *runnable_copy = runnable;

    push_queue(pool->waiting, runnable_copy);

    if ((err = pthread_cond_signal(&(pool->empty))) != 0) {
        syserr(err, "Conditional signal failed");
    }

    return 0;
}
