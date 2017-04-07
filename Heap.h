#ifndef _HEAP_H_
#define _HEAP_H_


typedef struct heapnode HeapNode;

typedef struct heapnode{
	char * number;
	float bill;
	HeapNode * Right;
	HeapNode * Left;
}HeapNode;

typedef struct heap{
	HeapNode *node;
	int count;
	double sum;
}Heap;
#endif