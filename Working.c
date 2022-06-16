#include "Working.h"

struct working_queue_t {

    Node head;
    Node tail;
    int curr_size;
    int thread_size
};

WorkingQueue workingQueueCreate(int thread_amount) {
    WorkingQueue queue = malloc(sizeof(struct working_queue_t));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = queue->tail = NULL;
    queue->curr_size = 0;
    queue->thread_size = thread_amount;  // assuming input size is valid
    return queue;
}

void workingPush(WorkingQueue queue, req r) {

    if (queue == NULL) {
        exit(1);
    }

    Node new_request = nodeCreate(r);
    if (new_request == NULL) {
        return;
    }

    pthread_t thread_id = pthread_self();
    ThreadInfo info = findRequestNode(thread_id);
    info->request_node = new_request;

    pthread_mutex_lock(&lock);
    if (queue->curr_size == 0) {
        queue->head = new_request;
        queue->tail = new_request;
    } else {
        // curr_size < thread_size
        new_request->next = queue->tail;
        (queue->tail)->prev = new_request;
        queue->tail = new_request;
    }
    queue->curr_size++;
    pthread_mutex_unlock(&lock);
}

req workingPopHead(WorkingQueue queue) {
    req data;

    if (!queue) {
        exit(1);
    }

    if (queue->curr_size == 0) {
        exit(1);
    }

    pthread_t thread_id = pthread_self();
    ThreadInfo info = findRequestNode(thread_id);
    Node request_node = info->request_node;
    req for_printing = request_node->r;


    pthread_mutex_lock(&lock);
    if (queue->curr_size == 1) {
        queue->head = queue->tail = NULL;
    } else if (request_node->prev == NULL) {
        queue->tail = request_node->next;
        request_node->next = NULL;
    } else if (request_node->next == NULL) {
        //This is the head
        queue->head = request_node->prev;
        request_node->prev = NULL;
    } else {
        //Somewhere in between
        (request_node->next)->prev = request_node->prev;
        (request_node->prev)->next = request_node->next;
        request_node->next = NULL;
        request_node->prev = NULL;
    }

    data = getNodeData(request_node);
    free(request_node); //TODO check if necessary
    info->request_node = NULL;
    total_handled--;
    queue->curr_size--;
    pthread_cond_signal(&is_full);
    pthread_mutex_unlock(&lock);

    return data;
}

int workingGetCurrSize(WorkingQueue queue) {
    return queue->curr_size;
}

int workingGetMaxSize(WorkingQueue queue) {
    return queue->thread_size;
}
