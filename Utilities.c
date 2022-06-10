#include "Utilities.h"


//! ****rio_t *getNodeData(Node node) {
int *getNodeData(Node node) {
    return node->request;
}

//! ****Node nodeCreate(rio_t *new_request) {
Node nodeCreate(int* new_request){
    Node node = malloc(sizeof(struct node_t));
    if (node == NULL) {
        return NULL;
    }
    printf("NodeCreate: the data is %d \n", *new_request);

    node->request = malloc(sizeof(*new_request));
    memcpy(node->request, new_request, sizeof(*new_request)); // TODO: make deep copy
    node->next = NULL;
    node->prev = NULL;
    return node;
    //TODO: insert id initialization
}

int createThreadArray(int thread_amount) {
    thread_arr = malloc(sizeof(struct thread_info_t) * thread_amount);
    if (!thread_arr) {
        return -1;
    }
    arr_size = thread_amount;
    return 1;
}

void addThreadInfo(pthread_t thread_id) {
    // * the point of this is that the master thread will have to call this function after creating every thread
    pthread_mutex_lock(&lock);
    static int index = 0;
    if (index == arr_size) {
        // * meaning we already initialized all the elements in the array
        return;
    }
    ThreadInfo info = malloc(sizeof(struct thread_info_t));

    if(!info){
        //! not good
        pthread_mutex_unlock(&lock);
        return;
    }
    info->thread_id = thread_id;
    info->request_node = NULL;
    thread_arr[index] = info;
    index++;
    pthread_mutex_unlock(&lock);

}

ThreadInfo findRequestNode(pthread_t thread_id) {
    for (int i = 0; i < arr_size; i++) {
            if (thread_arr[i]->thread_id == thread_id) {
                return thread_arr[i];
        }
    }
    //! if got here, something's not right!
    return NULL;
}
