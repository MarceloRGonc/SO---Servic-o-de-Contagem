CC = gcc
CFLAGS = -ansi -Wall -Wextra -Wno-unused-result -pedantic -O3 -std=c99

#counter: Commons.o Map.o LinkedList.o Data.o Counter.c Counter.h
#	$(CC) Commons.o Map.o LinkedList.o Data.o Counter.c -o counter 

all: server client

server: Commons.o Map.o LinkedList.o Counter.o Data.o Server.c
	$(CC) Server.c -o server Commons.o Map.o LinkedList.o Data.o Counter.o 

client: Commons.o Client.c Biblioteca.o
	$(CC) Client.c -o client Commons.o Biblioteca.o

Biblioteca.o : Biblioteca.c Biblioteca.h
	$(CC) -c Biblioteca.c

Commons.o : Commons.h Commons.c
	$(CC) -c Commons.c

LinkedList.o : LinkedList.c LinkedList.h
	$(CC) -c LinkedList.c

Map.o : Map.c Map.h
	$(CC) -c Map.c

Data.o : Data.h Data.c
	$(CC) -c Data.c

Counter.o : Counter.c Counter.h
	$(CC) -c Counter.c

clean:
	rm *.o
	rm server
	rm client

