#ifndef __REQUEST_H__



#include "segel.h"


typedef struct stats_t {

    struct  timeval arrival_time;
    struct  timeval dispatch_interval;
    int handler_thread_id;
    int handler_thread_req_count;
    int handler_thread_static_req_count;
    int handler_thread_dynamic_req_count;

} stats;

void requestHandle(req r, stats *s);

#endif
