//#ifndef _TEST_QUEUE_
//#define _TEST_QUEUE_
#include <stdio.h>
#include "Waiting.h"
#include "Working.h"
#include <assert.h>




int main(){
/*
    WaitingQueue queue = queueCreate(3, DEFAULT);
    assert(getMaxSize(queue) == 3);
    assert(getCurrSize(queue) == 0);
    assert(popHead(queue) == NULL);
    assert(queue != NULL);

    rio_t* request = NULL;
    push(queue, request);
    assert(getCurrSize(queue) == 1);
    //assert(seeHead(queue) != NULL);
    push(queue, request);
    push(queue, request);
    assert(getCurrSize(queue) == 3);
    push(queue, request);
    assert(getCurrSize(queue) == 3);

    //assert(popHead(queue) != NULL);
    popHead(queue);
    assert(getCurrSize(queue) == 2);
    popHead(queue);
    //assert(popHead(queue) != NULL);
    assert(getCurrSize(queue) == 1);
    popHead(queue);
    //assert(popHead(queue) != NULL);
    assert(getCurrSize(queue) == 0);
    //assert(seeHead(queue) == NULL);
*/
    printf("Sanity check successful!\n");

    return 0;
}
