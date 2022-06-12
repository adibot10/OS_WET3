#
# To compile, type "make" or make "all"
# To remove files, type "make clean"
#
OBJS = server.o request.o segel.o client.o locking_stuff.o Utilities.o Waiting.o Working.o
TARGET = server

CC = gcc
CFLAGS = -g -Wall

LIBS = -lpthread 

.SUFFIXES: .c .o 

all: server client output.cgi
	-mkdir -p public
	-cp output.cgi favicon.ico home.html public

server: server.o request.o segel.o locking_stuff.o Utilities.o Waiting.o Working.o
	$(CC) $(CFLAGS) -o server server.o request.o segel.o locking_stuff.o Utilities.o Waiting.o Working.o -lm -Wformat-overflow=0 $(LIBS)

client: client.o segel.o
	$(CC) $(CFLAGS) -o client client.o segel.o -Wformat-overflow=0

output.cgi: output.c
	$(CC) $(CFLAGS) -o output.cgi output.c

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f $(OBJS) server client output.cgi
	-rm -rf public
