#ifndef OS_WET3_LOCKING_STUFF_H
#define OS_WET3_LOCKING_STUFF_H
#include "segel.h"

pthread_mutex_t lock;
pthread_cond_t is_empty;
pthread_cond_t is_full;
int total_handled;

int init_stuff();



#endif //OS_WET3_LOCKING_STUFF_H
