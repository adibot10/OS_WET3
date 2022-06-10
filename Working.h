#ifndef OS_WET3_WORKING_H
#define OS_WET3_WORKING_H

#include <stdlib.h>
#include "Utilities.h"


typedef struct working_queue_t *WorkingQueue;


WorkingQueue workingQueueCreate(int thread_amount);
void workingPush(WorkingQueue queue, rio_t *request);
rio_t* workingSeeHead(WorkingQueue queue);
rio_t* workingPopHead(WorkingQueue queue);
int workingGetCurrSize(WorkingQueue queue);
int workingGetMaxSize(WorkingQueue queue);


#endif //OS_WET3_WORKING_H
