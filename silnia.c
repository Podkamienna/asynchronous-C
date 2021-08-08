#include "future.h"
#include "threadpool.h"
#include "err.h"

#include <stdio.h>
#include <inttypes.h>
#include <malloc.h>

const int THREAD_COUNT = 3;

typedef struct arguments {
    uint64_t result;
    int current_number;
} arguments_t;

int min(int a, int b) {
    if (a < b) {
        return a;
    }

    return b;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }

    return b;
}

void *callable(void *arg, __attribute__((unused)) size_t argsz, __attribute__((unused)) size_t *ressz) {
    if (arg == NULL) {
        return NULL;
    }

    arguments_t *arguments = (arguments_t *) arg;

    if (arguments->current_number == 0) {
        arguments->result = 1;
    } else {
        arguments->result *= arguments->current_number;
    }

    arguments->current_number += 1;

    return arguments;
}

void calculate_intervals(thread_pool_t *pool, int lower_bound, int upper_bound, future_t *future) {
    int size = upper_bound - lower_bound;

    arguments_t *arg_callable = malloc(sizeof(*arg_callable));
    *arg_callable = (arguments_t) {1, lower_bound};

    if (async(pool, future, (callable_t) {callable, arg_callable, sizeof(arguments_t)}) != 0) {
        fatal("Async failed.");
    }

    for (int i = 1; i < size; i++) {
        if (map(pool, future, future, callable) != 0) {
            fatal("Map failed.");
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);

    if (n < 0) {
        fatal("n must be nonnegative.");
    }

    if (n == 1 || n == 0) {
        printf("%d\n", 1);

        return 0;
    }

    thread_pool_t pool;
    if (thread_pool_init(&pool, THREAD_COUNT) != 0) {
        fatal("Thread pool initialization failed.");
    }

    int thread_count = min(THREAD_COUNT * 2, n);

    int bounds[thread_count + 1];
    future_t futures[thread_count];
    uint64_t result = 1;

    for (int i = 0; i < thread_count; i++) {
        bounds[i] = i * (n / thread_count);
    }

    bounds[thread_count] = n + 1; //The intervals are open from above

    for (int i = 0; i < thread_count; i++) {
        calculate_intervals(&pool, bounds[i], bounds[i + 1], &futures[i]);
    }

    for (int i = 0; i < thread_count; i++) {
        arguments_t *arguments = await(&futures[i]);
        result *= arguments->result;
        free(arguments);
    }

    printf("%" PRIu64 "\n", result);

    thread_pool_destroy(&pool);
}

