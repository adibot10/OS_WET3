#ifndef OS_WET3_WAITING_H
#define OS_WET3_WAITING_H
#include <stdlib.h>
#include "Utilities.h"


typedef struct waiting_queue_t *WaitingQueue;

typedef enum policy_y {
    DEFAULT,
    BLOCK,
    TAIL,
    RANDOM,
    HEAD

} Policy;

WaitingQueue queueCreateWaiting(int input_size, Policy policy);

//! **** need to change
//void pushWaiting(WaitingQueue queue, rio_t *request);
//rio_t* seeHeadWaiting(WaitingQueue queue);
//rio_t* popHeadWaiting(WaitingQueue queue);

void pushWaiting(WaitingQueue queue, int *request);
int* seeHeadWaiting(WaitingQueue queue);
int* popHeadWaiting(WaitingQueue queue);

int getCurrSizeWaiting(WaitingQueue queue);
int getMaxSizeWaiting(WaitingQueue queue);

#endif //OS_WET3_WAITING_H
