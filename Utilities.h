#ifndef OS_WET3_NODE_H
#define OS_WET3_NODE_H
#include "segel.h"
#include "locking_stuff.h"


typedef struct node_t *Node;
struct node_t {
    //! ****rio_t *request; //data
    int* request; //! change back
    struct node_t *next;
    struct node_t *prev;
    //TODO: when doing statistics, find out how to get id of thread inside Node
};

//! ****Node nodeCreate(rio_t *new_request);
Node nodeCreate(int* new_request); //!change back
//! ****rio_t *getNodeData(Node node);
int* getNodeData(Node node); //! change back


typedef struct thread_info_t *ThreadInfo;
struct thread_info_t{
    pthread_t thread_id;
    Node request_node;
};

ThreadInfo* thread_arr;
int arr_size;
int createThreadArray(int thread_amount);
void addThreadInfo(pthread_t thread_id);
ThreadInfo findRequestNode(pthread_t thread_id);

#endif //OS_WET3_NODE_H
