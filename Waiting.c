#include "Waiting.h"

struct waiting_queue_t {
    Node head;
    Node tail;
    int curr_size;
    int max_size;
    Policy policy;
};


WaitingQueue queueCreateWaiting(int input_size, Policy policy) {
    //assuming input size is valid
    WaitingQueue queue = malloc(sizeof(struct waiting_queue_t));
    if (queue == NULL) {
        printf("Not good, queueCreateWaiting failed!\n");
        fflush(stdout);
        exit(1);
    }

    queue->head = queue->tail = NULL;
    queue->curr_size = 0;
    queue->max_size = input_size;
    queue->policy = policy;
    return queue;
}

void pushWaiting(WaitingQueue queue, req r) {

    if (queue == NULL) {
        printf("Not good, pushWaiting failed!\n");
        fflush(stdout);
        exit(1);
    }

    Node new_request = nodeCreate(r);
    if (new_request == NULL) {
        return;
    }

    pthread_mutex_lock(&lock);
    if (queue->curr_size == 0) {
        queue->head = new_request;
        queue->tail = new_request;

    } else {
        //creating the node to insert to the end
        new_request->next = queue->tail;
        (queue->tail)->prev = new_request;
        queue->tail = new_request;
    }
    queue->curr_size++;
    total_handled++;
    pthread_cond_signal(&is_empty);
    pthread_mutex_unlock(&lock);
    return;
}


req popHeadWaiting(WaitingQueue queue, bool is_main_thread) {

    if (!queue) {
        printf("popHeadWaiting: queue bad");
        fflush(stdout);
        pthread_mutex_unlock(&lock);
        exit(1);
    }

    if(!is_main_thread) {
        pthread_mutex_lock(&lock);
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
    req data = getNodeData(temp);
    free(temp);

    if(!is_main_thread){
        pthread_mutex_unlock(&lock);
    }

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
    waiting_queue->curr_size--;
    total_handled--;
    free(temp);
    return r;
}

