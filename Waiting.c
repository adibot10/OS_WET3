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

//! **** need to change
//void pushWaiting(WaitingQueue queue, rio_t *request)
void pushWaiting(WaitingQueue queue, int *request) {
    pthread_mutex_lock(&lock);
    if (queue == NULL) {
        pthread_mutex_unlock(&lock);
        return;
    }

    Node new_request = nodeCreate(request);
    if (new_request == NULL) {
        pthread_mutex_unlock(&lock);
        return;
    }
    printf("WaitingQueue: entering request number %d \n", *(new_request->request)); //**** remove
    if (queue->curr_size == 0) {
        queue->head = new_request;
        queue->tail = new_request;
        queue->curr_size++;
        total_handled++;
        pthread_cond_signal(&is_empty);
        pthread_mutex_unlock(&lock);
        return;
    }

    if (total_handled < queue->max_size) {
        //creating the node to insert to the end
        new_request->next = queue->tail;
        (queue->tail)->prev = new_request;
        queue->tail = new_request;
        queue->curr_size++;
        pthread_mutex_unlock(&lock);
        return;
    }
    //reaching here must mean total_handled == queue->max_size

    if (queue->policy == DEFAULT) {
        printf("WaitingQueue: ignoring request number %d\n", *request);

    } else if (queue->policy == BLOCK) {
        //TODO: implement

    } else if (queue->policy == TAIL) {
        //TODO: implement

    } else if (queue->policy == RANDOM) {
        //TODO: implement

    } else if(queue->policy == HEAD){
        //TODO: implement
    }
    pthread_mutex_unlock(&lock);
    return;
}


rio_t *seeHeadWaiting(WaitingQueue queue) {
    if (!queue) {
        return NULL;
    }
    return getNodeData(queue->head);
}

//! **** need to change
//rio_t *popHeadWaiting(WaitingQueue queue)
int *popHeadWaiting(WaitingQueue queue) {
    pthread_mutex_lock(&lock);

    if (!queue) {
        pthread_mutex_unlock(&lock);
        return NULL;
    }
    while (0 == queue->curr_size) {
        pthread_cond_wait(&is_empty, &lock);
        }
    Node temp = queue->head;

    if (1 == queue->curr_size) {
        queue->head = queue->tail = NULL;

    } else {
        queue->head = (queue->head)->prev;
        (queue->head)->next = NULL;
    }
    queue->curr_size--;
    rio_t *data = getNodeData(temp);
    free(temp);
    //printf("WaitingQueue: popping request number %d \n", *data); //**** remove
    pthread_mutex_unlock(&lock);
    return data;

}

int getCurrSizeWaiting(WaitingQueue queue) {
    return queue->curr_size;
}

int getMaxSizeWaiting(WaitingQueue queue) {
    return queue->max_size;
}
