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
        printf("Not good, queueCreateWaiting failed!\n");
        exit(1);
    }
    queue->head = queue->tail = NULL;
    queue->curr_size = 0;
    queue->max_size = input_size;
    queue->policy = policy;
    return queue;
}

//! **** need to change
//void pushWaiting(WaitingQueue queue, rio_t *request)
void pushWaiting(WaitingQueue queue, req r) {

    if (queue == NULL) {
        printf("Not good, pushWaiting failed!\n");
        exit(1);
    }

    Node new_request = nodeCreate(r);
    if (new_request == NULL) {
        return;
    }
    if (queue->curr_size == 0) {
        queue->head = new_request;
        queue->tail = new_request;
        queue->curr_size++;
        total_handled++;
        pthread_cond_signal(&is_empty);
        return;
    }

    if (total_handled < queue->max_size) {
        //creating the node to insert to the end
        new_request->next = queue->tail;
        (queue->tail)->prev = new_request;
        queue->tail = new_request;
        queue->curr_size++;
        return;
    }
    //reaching here must mean total_handled == queue->max_size

    return;
}


req seeHeadWaiting(WaitingQueue queue) {
    req r;
    r.connfd = -1;
    if (!queue) {

        exit(1);
        //return r;
    }
    return getNodeData(queue->head);
}

req popHeadWaiting(WaitingQueue queue, bool is_main_thread) {
    req r;
    r.connfd = -1;
    if(!is_main_thread) {
        pthread_mutex_lock(&lock);
    }
    if (!queue) {
        printf("popHeadWaiting: queue bad");
        pthread_mutex_unlock(&lock);
        exit(1);
        //return r;
    }
    //printf("curr_size WaitingQueue: %d and thread id: %d\n", queue->curr_size, (int) pthread_self());
    fflush(stdout);

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
    req data = getNodeData(temp);
    free(temp);
    pthread_cond_signal(&is_full);
    pthread_mutex_unlock(&lock);
    return data;
}

int getCurrSizeWaiting(WaitingQueue queue) {
    return queue->curr_size;
}

int getMaxSizeWaiting(WaitingQueue queue) {
    return queue->max_size;
}


req popIndexWaiting(WaitingQueue waiting_queue,int queue_index){
    Node temp = waiting_queue->tail;

    while(queue_index > 0){
        temp = temp->next;
        queue_index--;
    }
    req r = temp->r;

    if(temp == waiting_queue->tail){
        waiting_queue->tail = temp->next;
        if(temp->next) {
            (temp->next)->prev = NULL;
        }
    }
    else if(temp == waiting_queue->head){
        waiting_queue->head = temp->prev;
        if(temp->prev){
            (temp->prev)->next = NULL;
        }
    }
    else {
        (temp->next)->prev = temp->prev;
        (temp->prev)->next = temp->next;
    }
    free(temp);
    return r;
}

