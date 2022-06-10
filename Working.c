#include "Working.h"

struct working_queue_t {

    Node head;
    Node tail;
    int curr_size;
    int thread_size;
    //TODO: something about threads

};


WorkingQueue workingQueueCreate(int thread_amount) {
    //assuming input size is valid
    WorkingQueue queue = malloc(sizeof(struct working_queue_t));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = queue->tail = NULL;
    queue->curr_size = 0;
    queue->thread_size = thread_amount;
    return queue;
}

// ****
//void workingPush(WorkingQueue queue, rio_t *request) {
void workingPush(WorkingQueue queue, int fd) {
    pthread_mutex_lock(&lock);
    if (queue == NULL) {
        pthread_mutex_unlock(&lock);
        return;
    }

    Node new_request = nodeCreate(fd);

    if (new_request == NULL) {
        pthread_mutex_unlock(&lock);
        return;
    }
    pthread_t thread_id = pthread_self();
    ThreadInfo info = findRequestNode(thread_id);
    info->request_node = new_request;

    if (queue->curr_size == 0) {
        queue->head = new_request;
        queue->tail = new_request;
        queue->curr_size++;
    }

    else {
    // curr_size < thread_size
        new_request->next = queue->tail;
        (queue->tail)->prev = new_request;
        queue->tail = new_request;
        queue->curr_size++;
    }
    //printf("WorkingQueue: pushing request number %d \n", *request); //**** remove
    pthread_mutex_unlock(&lock);

    //reaching here must mean queue->curr_size == queue->thread_size

}

// ****
//rio_t *workingSeeHead(WorkingQueue queue)
int workingSeeHead(WorkingQueue queue) {
    if (!queue) {
        return -1;
    }
    return getNodeData(queue->head);
}
// ****
//rio_t *workingPopHead(WorkingQueue queue)
int workingPopHead(WorkingQueue queue) {
    pthread_mutex_lock(&lock);
    if (!queue) {
        pthread_mutex_unlock(&lock);
        return -1;
    }

    if (queue->curr_size == 0) {
        //!not good if made it to here
        pthread_mutex_unlock(&lock);
        return -1;
    }

    if (queue->curr_size == 1) {
        queue->head = queue->tail = NULL;
    }

    pthread_t thread_id = pthread_self();
    ThreadInfo info = findRequestNode(thread_id);
    Node request_node = info->request_node;

    if(request_node->prev == NULL){
        //This is the tail
        queue->tail = request_node->next;
        request_node->next = NULL;
    }

    else if(request_node->next == NULL){
        //This is the head
        queue->head = request_node->prev;
        request_node->prev = NULL;
    }

    else{
        //Somewhere in between
        (request_node->next)->prev = request_node->prev;
        (request_node->prev)->next = request_node->next;
        request_node->next = NULL;
        request_node->prev = NULL;
    }

    queue->curr_size--;
    int data = getNodeData(request_node);
    //printf("WorkingQueue: taken care of request number %d \n", *(request_node->request)); //**** remove
    info->request_node = NULL;
    free(request_node);
    total_handled--;

    pthread_mutex_unlock(&lock);
    return data;

}

int workingGetCurrSize(WorkingQueue queue) {
    return queue->curr_size;
}

int workingGetMaxSize(WorkingQueue queue) {
    return queue->thread_size;
}
