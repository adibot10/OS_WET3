#include "segel.h"
#include "request.h"
#include "Waiting.h"
#include "Working.h"
// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
//number of args greater or equal to 2,
//takes the second argument and saves it in port
void getargs(int *port, int *threads, int *queue_size, char *schedalg, int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s <port> <threads> <queue_size> <schedalg>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queue_size = atoi(argv[3]);
    schedalg = (char *) malloc(sizeof(char) * (strlen(argv[4]) + 1));
    strcpy(schedalg, argv[4]);
    if(*threads <= 0)
    {
        fprintf(stderr, "Number of threads must be a positive number\n");
        exit(1);
    }
    if(*queue_size <= 0)
    {
        fprintf(stderr, "Queue size must be a positive number\n");
        exit(1);
    }
    if(strcmp(schedalg, "block") != 0 && strcmp(schedalg, "dt") != 0 && strcmp(schedalg, "dh") != 0 && strcmp(schedalg, "random") != 0)
    {
        fprintf(stderr, "Invalid Schedalg\n");
        exit(1);
    }

}

void *threadHandleRequests(void *param) {
    Params params = (Params) param;
    addThreadInfo(pthread_self());
    int fd;

    while (1) {
        fd = popHeadWaiting(params->wait);
        workingPush(params->work, fd);
        requestHandle(fd);
        workingPopHead(params->work);
        Close(connfd);
    }
}

Policy check_policy(char * schedalg)
{
    Policy policy = DEFAULT;
    if(strcmp(schedalg, "block"))
    {
        policy = BLOCK;
    }
    else if(strcmp(schedalg, "dt"))
    {
        policy = TAIL;
    }
    else if(strcmp(schedalg, "dh"))
    {
        policy = HEAD;
    } else if (strcmp(schedalg, "random"))
    {
        policy = RANDOM;
    }
    return policy;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, queue_size;
    struct sockaddr_in clientaddr;
    char * schedalg;
    getargs(&port, &threads, &queue_size, schedalg, argc, argv);
    init_stuff();
    int thread_size = threads;
    createThreadArray(thread_size);
    WorkingQueue work_queue = workingQueueCreate(thread_size);
    Policy policy = check_policy(schedalg);
    WaitingQueue waiting_queue = queueCreateWaiting(thread_size, policy);

    //creating the threads
    pthread_t thread_id;
    Params parameters = paramsCreate(work_queue, waiting_queue);

    for (int i = 0; i < thread_size; i++) {
        pthread_create(&thread_id, NULL, threadHandleRequests, (void *) parameters);
    }


    // 
    // HW3: Create some threads...
    //

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

    pushWaiting(waiting_queue, connfd);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 

    }

}


    


 
