#include "Waiting.h"


struct waiting_queue_t {

    Node head;
    Node tail;
    int curr_size;
    int max_size;
    Policy policy;
    //TODO: something about threads

};


WaitingQueue queueCreateWaiting(int input_size, Policy policy) {
    //assuming input size is valid
    WaitingQueue queue = malloc(sizeof(struct waiting_queue_t));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = queue->tail = NULL;
    queue->curr_size = 0;
    queue->max_size = input_size;
    queue->policy = policy;
    return queue;
}


void pushWaiting(WaitingQueue queue, rio_t *request) {
    if (queue == NULL) {
        return;
    }

    if (queue->curr_size == 0) {
        Node first = nodeCreate(request);
        if (first == NULL) {
            return;
        }
        queue->head = first;
        queue->tail = first;
        queue->curr_size++;

        return;
    }

    if (queue->curr_size < queue->max_size) {
        //creating the node to insert to the end
        Node first = nodeCreate(request);
        if (first == NULL) {
            return;
        }
        first->next = queue->tail;
        (queue->tail)->prev = first;
        queue->tail = first;
        queue->curr_size++;
        return;
    }
    //reaching here must mean queue->curr_size == queue->max_size

    if (queue->policy == DEFAULT) {
        return;
    }

    if (queue->policy == BLOCK) {
        //TODO: implement
        return;
    }
    if (queue->policy == TAIL) {
        //TODO: implement
        return;
    }
    if (queue->policy == RANDOM) {
        //TODO: implement
        return;
    }
    if (queue->policy == HEAD) {
        //TODO: implement
        return;
    }
}


rio_t *seeHeadWaiting(WaitingQueue queue) {
    if (!queue) {
        return NULL;
    }
    return getNodeData(queue->head);
}

rio_t *popHeadWaiting(WaitingQueue queue) {
    if (!queue) {
        return NULL;
    }

    if (queue->curr_size == 0) {
        //something with threads
        return NULL;
    }
    Node temp = queue->head;

    if (queue->curr_size == 1) {
        queue->head = queue->tail = NULL;
    } else {
        queue->head = (queue->head)->prev;
        (queue->head)->next = NULL;
    }
    queue->curr_size--;
    rio_t *data = getNodeData(temp);
    free(temp);

    return data;

}

int getCurrSizeWaiting(WaitingQueue queue) {
    return queue->curr_size;
}

int getMaxSizeWaiting(WaitingQueue queue) {
    return queue->max_size;
}
