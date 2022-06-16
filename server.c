#include "segel.h"#include "request.h"#include "Waiting.h"#include "Working.h"#define POPPING_PERCENT 30//// server.c: A very, very simple web server//// To run://  ./server <portnum (above 2000)>//// Repeatedly handles HTTP requests sent to this port number.// Most of the work is done within routines written in request.c//// HW3: Parse the new arguments too//number of args greater or equal to 2,//takes the second argument and saves it in porttypedef struct params_t *Params;struct params_t{    WorkingQueue work;    WaitingQueue wait;    int num_threads;};Params paramsCreate(WorkingQueue work_queue, WaitingQueue waiting_queue, int num_threads){    Params param = malloc(sizeof(struct params_t));    if (param == NULL)    {        return NULL;    }    param->work = work_queue;    param->wait = waiting_queue;    param->num_threads = num_threads;    return param;}void getargs(int *port, int *threads, int *queue_size, char **schedalg, int argc, char *argv[]){    if (argc < 5)    {        fprintf(stderr, "Usage: %s <port> <threads> <queue_size> <schedalg>\n", argv[0]);        exit(1);    }    *port = atoi(argv[1]);    *threads = atoi(argv[2]);    *queue_size = atoi(argv[3]);    *schedalg = (char *) malloc(sizeof(char) * (strlen(argv[4]) + 1));    strcpy(*schedalg, argv[4]);    if (*threads <= 0)    {        fprintf(stderr, "Number of threads must be a positive number\n");        exit(1);    }    if (*queue_size <= 0)    {        fprintf(stderr, "Queue size must be a positive number\n");        exit(1);    }    if (strcmp(*schedalg, "block") != 0 && strcmp(*schedalg, "dt") != 0 && strcmp(*schedalg, "dh") != 0 &&        strcmp(*schedalg, "random") != 0)    {        fprintf(stderr, "Invalid Schedalg\n");        exit(1);    }}void *threadHandleRequests(void *param){    Params params = (Params) param;    int id = addThreadInfo(pthread_self());    stats s[params->num_threads];    s[id].handler_thread_id = id; //TODO check return value for -1    s[id].handler_thread_req_count = 0;    s[id].handler_thread_dynamic_req_count = 0;    s[id].handler_thread_static_req_count = 0;    req r;    while (1)    {        r = popHeadWaiting(params->wait, false);        //printf("returned from popHeadWaiting with req %d and thread id of %d\n", r.connfd, (int)pthread_self());//        fflush(stdout);        if (r.connfd != -1)        {//            printf("[%d] started WorkingPush\n", id);//            fflush(stdout);            workingPush(params->work, r);//            printf("[%d] ended WorkingPush\n", id);//            fflush(stdout);//            printf("[%d] started requestHandle\n", id);//            fflush(stdout);            requestHandle(r, &(s[id]));//            printf("[%d] ended requestHandle\n", id);//            fflush(stdout);//            printf("[%d] started workingPopHead\n", id);//            fflush(stdout);            workingPopHead(params->work);//            printf("[%d] ended workingPopHead\n", id);//            fflush(stdout);            Close(r.connfd);        }        else{            printf("Very bad!\n");            fflush(stdout);            exit(1);        }    }}Policy check_policy(char *schedalg){    Policy policy = DEFAULT;    if (strcmp(schedalg, "block") == 0)    {        policy = BLOCK;        printf("got into block\n");    } else if (strcmp(schedalg, "dt") == 0)    {        policy = TAIL;        printf("got into drop tail\n");    } else if (strcmp(schedalg, "dh") == 0)    {        policy = HEAD;        printf("got into drop head\n");    } else if (strcmp(schedalg, "random") == 0)    {        policy = RANDOM;        printf("got into random\n");    }    fflush(stdout);    return policy;}void popRandom(WaitingQueue waiting_queue){    int queue_index;    int curr_queue_size = getCurrSizeWaiting(waiting_queue);    double delete_amount_double = (double)curr_queue_size * ((double)POPPING_PERCENT / (double)100);    int delete_amount = (int) ceil(delete_amount_double);    req r;//    printf("the delete_double is %f\n", delete_amount_double);////    printf("popping %d elements\n", delete_amount);//    fflush(stdout);    //assuming 0 is tail of queue, size - 1 is head of queue    for (int i = 0; i < delete_amount && curr_queue_size > 0; i++)    {        queue_index = rand() % curr_queue_size;        r = popIndexWaiting(waiting_queue, queue_index);        curr_queue_size--;        Close(r.connfd);    }}int main(int argc, char *argv[]){    int listenfd, connfd, port, clientlen, threads_amount, queue_size;    req r;    bool almost_queue_size = false;    struct sockaddr_in clientaddr;    char *schedalg;    getargs(&port, &threads_amount, &queue_size, &schedalg, argc, argv);    init_stuff();    int thread_size = threads_amount;    createThreadArray(thread_size);    WorkingQueue work_queue = workingQueueCreate(thread_size);    Policy policy = check_policy(schedalg);    WaitingQueue waiting_queue = queueCreateWaiting(queue_size, policy);    srand((unsigned) time(0)); //initialize random number generator    //creating the threads    pthread_t thread_id;    Params parameters = paramsCreate(work_queue, waiting_queue, threads_amount);    int req_num = 1; //* *    for (int i = 0; i < thread_size; i++)    {        pthread_create(&thread_id, NULL, threadHandleRequests, (void *) parameters);    }    listenfd = Open_listenfd(port);    printf("%s\n",schedalg);    while (1)    {        clientlen = sizeof(clientaddr);        /** not sure if using locks here is starvation or busy waiting, need to check         on the other hand, if I don't use locks I won't be able to check the global variable         without fear of context switch*///        printf("waiting for request num: %d\n", req_num);//        fflush(stdout);        r.connfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t * ) & clientlen);        if(r.connfd < 0){            exit(1);        }//        printf("got request num %d. preparing to lock:\n", req_num);//        fflush(stdout);////        printf("locked request num: %d\n", req_num);//        fflush(stdout);        if (total_handled == queue_size - 1) {            almost_queue_size = true;//            printf("got to dangerous size with req num: %d\n", req_num);//            fflush(stdout);            if (gettimeofday(&r.arrival_time, NULL) == -1) {                return -1;            }            pushWaiting(waiting_queue, r);//            printf("finished pushing, total_handled == queue_size\n");//            fflush(stdout);            pthread_mutex_lock(&lock);            while (policy == BLOCK && total_handled == queue_size) {//                printf("into while, not good :-(\n");//                fflush(stdout);                //wait until a request is done, need while because of condition variable                pthread_cond_wait(&is_full, &lock);            }//            printf("got over while, did not see that coming\n");//            fflush(stdout);            goto push_and_unlock;        }        pthread_mutex_lock(&lock);        if (total_handled < queue_size) {            goto push_and_unlock;        }        //reaching here means that total_handled == queue_size        switch (policy)        {            //Sorry Adi, couldn't stop myself            case TAIL:                Close(r.connfd);                pthread_mutex_unlock(&lock);                continue;            case RANDOM:/*                printf("got into random with req num: %d\n", req_num);                fflush(stdout);*///                printf("queue full, using random policy!\n");//                fflush(stdout);                if (0 == getCurrSizeWaiting(waiting_queue))                {//                    printf("closing new connection\n");//                    fflush(stdout);                    Close(r.connfd);                    pthread_mutex_unlock(&lock);                    continue;                }//                printf("popping 30 percent\n");//                fflush(stdout);                popRandom(waiting_queue);                break;            case HEAD://                printf("got to head with request number %d\n", req_num);//                fflush(stdout);                if (0 == getCurrSizeWaiting(waiting_queue))                {                    Close(r.connfd);                    pthread_mutex_unlock(&lock);                    continue;                }                req pop_head_req = popHeadWaiting(waiting_queue, true); //! changed                Close(pop_head_req.connfd); //! changed                total_handled--; //! changed                break;            default:                //DEFAULT only, BLOCK already handled                continue;        }    push_and_unlock:        pthread_mutex_unlock(&lock);        if (gettimeofday(&r.arrival_time, NULL) == -1)        {            return -1;        }        if(!almost_queue_size){//            printf("going to push request \n");//            fflush(stdout);            pushWaiting(waiting_queue, r);        }        pthread_cond_signal(&is_empty);//        printf("the fd of current request is: %d\n", r.connfd);//        printf("current size of WaitingQueue: %d\n", getCurrSizeWaiting(waiting_queue));//        printf("current size of WorkingQueue: %d\n", workingGetCurrSize(work_queue));//        //printf("current amount being taken care of: %d\n", total_handled);//        printf("put request num %d in queue. unlocking: \n", req_num);//        printThreadsInfo();//        fflush(stdout);//        req_num++;        almost_queue_size = false;    }}     