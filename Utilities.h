#ifndef OS_WET3_NODE_H
#define OS_WET3_NODE_H
#include "segel.h"
#include "locking_stuff.h"
#include "stdbool.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>


typedef struct node_t *Node;
struct node_t {
    req r;
    struct node_t *next;
    struct node_t *prev;
};


Node nodeCreate(req r);

req getNodeData(Node node);


typedef struct thread_info_t *ThreadInfo;
struct thread_info_t{
    pthread_t thread_id;
    Node request_node;
};

ThreadInfo* thread_arr;
int arr_size;
int createThreadArray(int thread_amount);
int addThreadInfo(pthread_t thread_id);
ThreadInfo findRequestNode(pthread_t thread_id);

#endif //OS_WET3_NODE_H
