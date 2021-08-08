#include "future.h"
#include "err.h"

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>

typedef void *(*function_t)(void *);

typedef struct arguments {
    sem_t semaphore;
    pthread_mutex_t lock;
    bool is_done;
    bool is_map;
    arguments_t *mapped_arguments;
    thread_pool_t *mapped_pool;
    void *result;
    size_t ressz;
    callable_t callable;
} arguments_t;

static int init_future(future_t *future) {
    if (future == NULL) {
        return -1;
    }

    future->arguments = malloc(sizeof(arguments_t));
    if (future->arguments == NULL) {
        return -1;
    }

    if (sem_init(&future->arguments->semaphore, 0, 0)) {
        free(future->arguments);

        return -1;
    }

    if (pthread_mutex_init(&future->arguments->lock, NULL) != 0) {
        if (sem_destroy(&future->arguments->semaphore) == -1) {
            syserr(errno, "Semaphore destroy failed.");
        }
        free(future->arguments);

        return -1;
    }

    future->arguments->is_done = false;
    future->arguments->is_map = false;

    return 0;
}

static void free_arguments(arguments_t *arguments) {
    int err;
    if ((err = pthread_mutex_destroy(&arguments->lock)) != 0) {
        syserr(err, "Mutex destroy failed.");
    }

    if (sem_destroy(&arguments->semaphore) == -1) {
        syserr(err, "Semaphore destroy failed.");
    }

    free(arguments);
}

static void destroy_future(future_t *future) {
    if (future == NULL) {
        return;
    }

    if (future->arguments != NULL) {
        free_arguments(future->arguments);
    }

    future->arguments = NULL;
}

static void runnable(void *arg, size_t argsz __attribute__((unused))) {
    int err;
    arguments_t *arguments = (arguments_t *) arg;
    arguments->result = (arguments->callable.function)(arguments->callable.arg, arguments->callable.argsz,
                                                       &arguments->ressz);

    if ((err = pthread_mutex_lock(&arguments->lock)) != 0) {
        syserr(err, "Mutex lock failed.");
    }

    arguments->is_done = true;

    if (!arguments->is_map) {
        if ((err = pthread_mutex_unlock(&arguments->lock)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        if (sem_post(&arguments->semaphore) == -1) {
            syserr(errno, "Semaphore post failed.");
        }
    } else {
        arguments_t *mapped_arguments = arguments->mapped_arguments;
        mapped_arguments->callable.arg = arguments->result;
        mapped_arguments->callable.argsz = arguments->ressz;

        if ((err = pthread_mutex_unlock(&arguments->lock)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        if (defer(arguments->mapped_pool, (runnable_t) {runnable, mapped_arguments, sizeof(*mapped_arguments)}) != 0) {
            fatal("Deferring mapped task failed.");
        }

        free_arguments(arguments);
    }
}

int async(thread_pool_t *pool, future_t *future, callable_t callable) {
    if (pool == NULL || future == NULL) {
        return -1;
    }

    if (init_future(future) != 0) {
        return -1;
    }

    future->arguments->callable = callable;

    defer(pool, (runnable_t) {runnable, future->arguments, sizeof(*future->arguments)});

    return 0;
}

int map(thread_pool_t *pool, future_t *future, future_t *from, void *(*function)(void *, size_t, size_t *)) {
    int err;
    int return_value = 0;

    if (pool == NULL || future == NULL || from == NULL || from->arguments == NULL) {
        return -1;
    }

    if (pthread_mutex_lock(&from->arguments->lock) != 0) {
        return -1;
    }

    if (from->arguments->is_done) {
        if ((err = pthread_mutex_unlock(&from->arguments->lock)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        callable_t callable = {function, from->arguments->result, from->arguments->ressz};
        destroy_future(from);

        return_value = async(pool, future, callable);
    } else {
        arguments_t *from_arguments = from->arguments;
        from->arguments = NULL;
        if (init_future(future) != 0) {
            fatal("Future initialization failed.");
        }
        from_arguments->is_map = true;
        from_arguments->mapped_arguments = future->arguments;
        from_arguments->mapped_pool = pool;
        future->arguments->callable = (callable_t) {function, NULL, 0};

        if ((err = pthread_mutex_unlock(&from_arguments->lock)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }
    }

    return return_value;
}

void *await(future_t *future) {
    if (future == NULL || future->arguments == NULL) {
        return NULL;
    }

    if (sem_wait(&future->arguments->semaphore) == -1) {
        syserr(errno, "Semaphore wait failed.");
    }

    void *result = future->arguments->result;

    destroy_future(future);

    return result;
}
