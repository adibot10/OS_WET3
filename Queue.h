//
// Created by Adi on 08/06/2022.
//


#ifndef WEBSERVER_FILES_QUEUE_H
#define WEBSERVER_FILES_QUEUE_H

#include <stdlib.h>
#include "segel.h"

struct node_t{
    rio_t request; //data
    struct node_t *next;
    struct node_t *prev;
    //TODO: when doing statistics, find out how to get id of thread inside Node
} Node;

struct queue_t{
    Node* head;
    Node* tail;
    int size;
    //TODO: something about threads
};


#endif //WEBSERVER_FILES_QUEUE_H
