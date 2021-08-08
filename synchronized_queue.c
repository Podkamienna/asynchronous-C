#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#include "synchronized_queue.h"
#include "err.h"

bool empty_queue(queue_t *queue) {
    if (queue == NULL) {
        return true;
    }

    int err;

    if ((err = pthread_mutex_lock(&queue->mutex)) != 0) {
        syserr(err, "Mutex lock failed.");
    }

    bool result = (queue->first == NULL);

    if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
        syserr(err, "Mutex unlock failed.");
    }

    return result;
}

queue_t *init_queue() {
    queue_t *new_queue = calloc(1, sizeof(queue_t));;
    if (new_queue == NULL) {
        return NULL;
    }

    if (pthread_mutex_init(&new_queue->mutex, 0) != 0) {
        free(new_queue);

        return NULL;
    }

    return new_queue;
}

int push_queue(queue_t *queue, void *value) {
    if (queue == NULL) {
        return -1;
    }
    int err;

    if (pthread_mutex_lock(&queue->mutex) != 0) {
        return -1;
    }

    queue_node_t *new_node = malloc(sizeof(queue_node_t));
    if (new_node == NULL) {
        if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        return -1;
    }

    new_node->value = value;
    new_node->next = NULL;

    if (queue->first == NULL) {
        queue->first = new_node;
        queue->last = queue->first;

        if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        return 0;
    }

    queue->last->next = new_node;
    queue->last = queue->last->next;

    if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
        syserr(err, "Mutex unlock failed.");
    }

    return 0;
}

void *top_queue(queue_t *queue) {
    if (queue == NULL) {
        return NULL;
    }

    int err;

    if ((err = pthread_mutex_lock(&queue->mutex)) != 0) {
        syserr(err, "Mutex lock failed.");
    }

    if (queue->first == NULL) {
        if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        return NULL;
    }

    runnable_t *result = queue->first->value;

    if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
        syserr(err, "Mutex unlock failed.");
    }

    return result;
}

void pop_queue(queue_t *queue) {
    if (queue == NULL) {
        return;
    }

    int err;

    if ((err = pthread_mutex_lock(&queue->mutex)) != 0) {
        syserr(err, "Mutex lock failed.");
    }

    if (queue->first == NULL) {
        if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
            syserr(err, "Mutex unlock failed.");
        }

        return;
    }

    queue_node_t *to_be_deleted = queue->first;
    queue->first = queue->first->next;

    free(to_be_deleted);

    if (queue->first == NULL) {
        queue->last = NULL;
    }

    if ((err = pthread_mutex_unlock(&queue->mutex)) != 0) {
        syserr(err, "Mutex unlock failed.");
    }
}

void destroy_queue(queue_t *queue) {
    if (queue == NULL) {
        return;
    }

    int err;

    if ((err = pthread_mutex_destroy(&queue->mutex)) != 0) {
        syserr(err, "Mutex destroy failed.");
    }

    queue_node_t *to_destroy = queue->first;
    queue_node_t *next;

    while (to_destroy != NULL) {
        next = to_destroy->next;
        free(to_destroy);

        to_destroy = next;
    }

    free(queue);
}