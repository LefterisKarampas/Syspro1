CC = gcc
CFLAGS = -g -Wall
TARGET = werhauz
HEADERS = Info.h

.PHONY: clean all

all: werhauz

main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o

Operations.o: Operations.c $(HEADERS)
	$(CC) $(CFLAGS) -c Operations.c -o Operations.o

Hash.h: Hash.c $(HEADERS)
	$(CC) $(CFLAGS)  -c Hash.c -o Hash.o

Bucket.o: Bucket.c $(HEADERS)
	$(CC) $(CFLAGS)  -c Bucket.c -o Bucket.o

Node.o: Node.c $(HEADERS)
	$(CC) $(CFLAGS)  -c Node.c -o Node.o

CDR_Node.o: CDR_Node.c $(HEADERS)
	$(CC) $(CFLAGS)  -c CDR_Node.c -o CDR_Node.o

Heap.o: Heap.c $(HEADERS)
	$(CC) $(CFLAGS)  -c Heap.c -o Heap.o -lm 

Caller_List.o: Caller_List.c $(HEADERS)
	$(CC) $(CFLAGS)  -c Caller_List.c -o Caller_List.o 

werhauz: main.o Hash.o Bucket.o Node.o CDR_Node.o Heap.o Caller_List.o Operations.o
	$(CC) $(CFLAGS)  main.o Hash.o Bucket.o Node.o CDR_Node.o Heap.o Caller_List.o Operations.o -o werhauz -lm

clean:
	rm -rf *.o $(TARGET) P


