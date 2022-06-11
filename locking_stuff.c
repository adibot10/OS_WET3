#include "locking_stuff.h"



int init_stuff() {
    // TODO: need to call at beginning of run
    int mut_init = pthread_mutex_init(&lock, NULL);
    if(mut_init != 0)
    {
        return -1;
    }
    pthread_cond_init(&is_empty, NULL); //always success
    pthread_cond_init(&is_full, NULL); //always success
    total_handled = 0;
    return 0;

}