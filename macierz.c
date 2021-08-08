#include "threadpool.h"
#include "err.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>

const int THREAD_COUNT = 4;
const unsigned long long NANOSECONDS_IN_SECONDS = 1000000000;
const unsigned long long NANOSECONDS_IN_MILLISECONDS = 1000000;

typedef struct arguments {
    long long *sum;
    pthread_mutex_t *lock;
    long long number;
    unsigned long long time; //time in nanoseconds
} arguments_t;

static void wait_and_add(void *args, size_t argsz) {
    int err;

    if (args == NULL || argsz == 0) {
        return;
    }

    arguments_t *arguments = (arguments_t *) args;

    const struct timespec time = {arguments->time / NANOSECONDS_IN_SECONDS, arguments->time % NANOSECONDS_IN_SECONDS};

    if (nanosleep(&time, NULL) != 0) {
        syserr(errno, "Error in sleep.");
    }

    if ((err = pthread_mutex_lock(arguments->lock)) != 0) {
        syserr(err, "Mutex lock failed.");
    }

    *arguments->sum += arguments->number;

    if ((err = pthread_mutex_unlock(arguments->lock)) != 0) {
        syserr(err, "Mutex unlock failed.");
    }
}

int main() {
    int err;
    size_t column_count, row_count;
    scanf("%lu %lu", &row_count, &column_count);

    long long tmp_number;
    unsigned long long tmp_time;

    arguments_t arguments[column_count][row_count];
    long long row_sum[row_count];
    pthread_mutex_t *locks = malloc(row_count * sizeof(pthread_mutex_t));
    if (locks == NULL) {
        fatal("Couldn't allocate memory");
    }

    for (size_t i = 0; i < row_count; i++) {
        if ((err = pthread_mutex_init(&locks[i], 0)) != 0) {
            syserr(err, "Semaphore init failed.");
        }
    }

    for (size_t i = 0; i < row_count; i++) {
        row_sum[i] = 0;
    }

    thread_pool_t thread_pool;
    if ((err = thread_pool_init(&thread_pool, THREAD_COUNT)) != 0) {
        syserr(err, "Pool initialization failed.");
    }

    for (size_t j = 0; j < row_count; j++) {
        for (size_t i = 0; i < column_count; i++) {
            scanf("%lld", &tmp_number);
            scanf("%llu", &tmp_time);
            if (tmp_time > ULLONG_MAX / NANOSECONDS_IN_MILLISECONDS) {
                fatal("Too long waiting time.");
            }

            arguments[i][j] = (arguments_t) {&row_sum[j], &locks[j], tmp_number, tmp_time * NANOSECONDS_IN_MILLISECONDS};
            defer(&thread_pool, (runnable_t) {wait_and_add, &arguments[i][j], sizeof(arguments_t)});
        }
    }

    thread_pool_destroy(&thread_pool);

    for (size_t i = 0; i < row_count; i++) {
        if ((err = pthread_mutex_destroy(&locks[i])) != 0) {
            syserr(err, "Mutex destroy failed");
        }
    }

    free(locks);

    for (size_t i = 0; i < row_count; i++) {
        printf("%lld\n", row_sum[i]);
    }
}

