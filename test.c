#ifndef _TEST_QUEUE_
#define _TEST_QUEUE_

#include <stdio.h>
#include "Waiting.h"
#include "Working.h"
#include <assert.h>
#include "locking_stuff.h"

#include <unistd.h>


void *test_thread(void *param) {
    sleep(5);
    Params params = (Params) param;
    addThreadInfo(pthread_self());
    int *request;

    while (1) {
        request = popHeadWaiting(params->wait);
        workingPush(params->work, request);
        sleep(1);
        workingPopHead(params->work);
    }
}


int main() {
    init_stuff();
    int thread_size = 10;
    createThreadArray(thread_size);
    WorkingQueue work_queue = workingQueueCreate(thread_size);
    WaitingQueue waiting_queue = queueCreateWaiting(thread_size, DEFAULT);
    pthread_t thread_id;
    Params parameters = paramsCreate(work_queue, waiting_queue);

    for (int i = 0; i < thread_size; i++) {
        pthread_create(&thread_id, NULL, test_thread, (void *) parameters);
    }

    int req1 = 1;
    int req2 = 2;
    int req3 = 3;
    int req4 = 4;
    int req5 = 5;
    int req6 = 6;
    int req7 = 7;
    int req8 = 8;
    int req9 = 9;
    int req10 = 10;
    int req11 = 11;

    int arr_size = 12;
    int req_arr[arr_size];
    for (int i = 0; i < arr_size; i++) {
        req_arr[i] = i;
    }
    for (int i = 0; i < arr_size; i++) {
        pushWaiting(waiting_queue, &i);
    }
    for(int i = 0; i < arr_size; i++){
        int* request = popHeadWaiting(waiting_queue);
        printf("request is : %d \n", *request);
    }
    sleep(6);


    printf("Sanity check successful!\n");

    return 0;
}

#endif
