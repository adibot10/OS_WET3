#ifndef OS_WET3_WAITING_H
#define OS_WET3_WAITING_H
#include <stdlib.h>
#include "Utilities.h"


typedef struct waiting_queue_t *WaitingQueue;

typedef enum policy_t {
    DEFAULT,
    BLOCK,
    TAIL,
    RANDOM,
    HEAD

} Policy;

WaitingQueue queueCreateWaiting(int input_size, Policy policy);


//void pushWaiting(WaitingQueue queue, rio_t *request);
//rio_t* seeHeadWaiting(WaitingQueue queue);
//rio_t* popHeadWaiting(WaitingQueue queue);

void pushWaiting(WaitingQueue queue, int fd);
int seeHeadWaiting(WaitingQueue queue);
int popHeadWaiting(WaitingQueue queue, bool is_main_thread);
int popIndexWaiting(WaitingQueue waiting_queue,int queue_index);
int getCurrSizeWaiting(WaitingQueue queue);
int getMaxSizeWaiting(WaitingQueue queue);

#endif //OS_WET3_WAITING_H
