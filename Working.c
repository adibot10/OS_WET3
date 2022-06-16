#include "Working.h"

struct working_queue_t {

    Node head;
    Node tail;
    int curr_size;
    int thread_size;
    //TODO: something about threads

};
int printWorkingQueue(WorkingQueue queue)
{
    Node ptr = queue->tail;
    int size = 0;
    while (ptr != NULL)
    {
        ptr = ptr->next;
        req q = ptr->r;
        //printf("%d->", q.connfd);

        //fflush(stdout);
        size++;
    }
    //printf("NULL\n");
    //fflush(stdout);
    return size;
}

WorkingQueue workingQueueCreate(int thread_amount) {
    //assuming input size is valid
    WorkingQueue queue = malloc(sizeof(struct working_queue_t));
    if (queue == NULL) {
        return NULL;
    }
    queue->head = queue->tail = NULL;
    queue->curr_size = 0;
    queue->thread_size = thread_amount;
    return queue;
}

// ****
//void workingPush(WorkingQueue queue, rio_t *request) {
void workingPush(WorkingQueue queue, req r) {
    pthread_mutex_lock(&lock);
    if (queue == NULL)
    {
        pthread_mutex_unlock(&lock);
        return;
    }

    FILE* fp = fopen("tests_with_prints", "a");
    fprintf(fp, "\n\n\nforgive me, but now pushin in working\n");
    fclose(fp);


    Node new_request = nodeCreate(r);
    if (new_request == NULL) {
        pthread_mutex_unlock(&lock);
        return;
    }
    pthread_t thread_id = pthread_self();
    ThreadInfo info = findRequestNode(thread_id);
    info->request_node = new_request;
    fp = fopen("tests_with_prints", "a");
    fprintf(fp, "pushing request number %d in working\n", ((info->request_node)->r).connfd);
    fclose(fp);

/*
    printf("PUSH-the Real size of the WorkingQueue is %d and the curr size is %d\n", printWorkingQueue(queue), workingGetCurrSize(queue));
    fflush(stdout);
*/
    if (queue->curr_size == 0) {
        queue->head = new_request;
        queue->tail = new_request;

    }

    else {
        // curr_size < thread_size
        new_request->next = queue->tail;
        (queue->tail)->prev = new_request;
        queue->tail = new_request;

    }
    queue->curr_size++;
/*
    printf("WorkingQueue: pushing request number %d\n", r.connfd); //**** remove
    fflush(stdout);
*/

    pthread_mutex_unlock(&lock);

    //reaching here must mean queue->curr_size == queue->thread_size

}

// ****
//rio_t *workingSeeHead(WorkingQueue queue)
req workingSeeHead(WorkingQueue queue) {
    req r;
    r.connfd = -1;
    if (!queue) {
        return r;
    }
    return getNodeData(queue->head);
}

// ****
req workingPopHead(WorkingQueue queue) {
    req data;
    pthread_mutex_lock(&lock);
    FILE* fp = fopen("tests_with_prints", "a");
    fprintf(fp, "\n\n\nforgive me, but now popping from working\n");
    fclose(fp);

//    printf("\n\n");
//    fflush(stdout);

    if (!queue) {
        fp = fopen("tests_with_prints", "a");
        fprintf(fp, "queue is null, apparently\n");
        fclose(fp);

        pthread_mutex_unlock(&lock);
        exit(1);
    }

/*
    printf("curr size in work so far WorkingQueue: %d\n", queue->curr_size);
    fflush(stdout);
*/
    //int s = printWorkingQueue(queue);
    //printf("POP-the Real size of the WorkingQueue is %d and the curr size is %d\n", s, queue->curr_size);
    //fflush(stdout);
    fp = fopen("tests_with_prints", "a");
    fprintf(fp, "checking got after !queue\n");
    fclose(fp);

    if (queue->curr_size == 0) {
        printf("not good, size is 0 when popping WorkQueue\n");
        fflush(stdout);
        fp = fopen("tests_with_prints", "a");
        fprintf(fp, "zero items in queue, not good\n");
        fclose(fp);
        //!not good if made it to here
        pthread_mutex_unlock(&lock);
        exit(1);
    }
    pthread_t thread_id = pthread_self();
    ThreadInfo info = findRequestNode(thread_id);
    Node request_node = info->request_node;
    fp = fopen("tests_with_prints", "a");
    fprintf(fp, "found node to remove for queue\n");
    fclose(fp);
//    printf("\n\nreached working pop head\n");
//    printf("popping req num: %d\n", (request_node->r).connfd);
//    printf("reached working pop head\n\n\n");
//    fflush(stdout);

    req for_printing = request_node->r;
    fp = fopen("tests_with_prints", "a");
    fprintf(fp, "just popping request number %d from working\n", for_printing.connfd);
    fclose(fp);

    if (queue->curr_size == 1) {
//        printf("size is 1 in WorkQueue, request number %d\n", for_printing.connfd);
//        fflush(stdout);
        queue->head = queue->tail = NULL;

    } else if (request_node->prev == NULL) {
        //This is the tail
//        printf("popping tail in WorkQueue, request number %d\n", for_printing.connfd);
//        fflush(stdout);
        queue->tail = request_node->next;
        request_node->next = NULL;

    } else if (request_node->next == NULL) {
        //This is the head

//        printf("popping head in WorkQueue, request number %d\n", for_printing.connfd);
//
//        fflush(stdout);
        queue->head = request_node->prev;
        request_node->prev = NULL;

    } else {
        //Somewhere in between
//        printf("popping middle in WorkQueue, request number %d\n", for_printing.connfd);
//        fflush(stdout);
        (request_node->next)->prev = request_node->prev;
        (request_node->prev)->next = request_node->next;
        request_node->next = NULL;
        request_node->prev = NULL;
    }

    //queue->curr_size--;
    data = getNodeData(request_node);
    //printf("WorkingQueue: taken care of request number %d \n", *(request_node->request)); //**** remove
    free(request_node); //TODO check if necessary
    info->request_node = NULL;
    total_handled--;
    queue->curr_size--;
    //printf("total handled to go WaitingQueue: %d\n", total_handled);
    pthread_cond_signal(&is_full);
    fp = fopen("tests_with_prints", "a");
    fprintf(fp, "just just sent signal to wake up main in waiting \n");
    fclose(fp);

    pthread_mutex_unlock(&lock);

    return data;

}

int workingGetCurrSize(WorkingQueue queue) {
    return queue->curr_size;
}

int workingGetMaxSize(WorkingQueue queue) {
    return queue->thread_size;
}
