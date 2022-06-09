#include "segel.h"
#include "request.h"

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


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, threads, queue_size;
    struct sockaddr_in clientaddr;
    char * schedalg;
    getargs(&port, &threads, &queue_size, schedalg, argc, argv);

    // 
    // HW3: Create some threads...
    //



    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	requestHandle(connfd);

	Close(connfd);
    }

}


    


 
