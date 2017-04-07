#ifndef _BUCKET_H_
#define _BUCKET_H_
#include "Node.h"

typedef struct bucket Bucket;

typedef struct bucket{
	Node * NodeArray;
	int max_entry;
	int count;
	Bucket *Next;
	Bucket *Previous;
}Bucket;

#endif