#include "locking_stuff.h"


int total_handled = 0;

int init_stuff() {
    int mut_init = pthread_mutex_init(&lock, NULL);
    int empty_init = pthread_cond_init(&empty, NULL);
    int full_init = pthread_cond_init(&full, NULL);
}