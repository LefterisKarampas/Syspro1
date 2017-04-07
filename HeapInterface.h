#ifndef _HEAP_INH_
#define _HEAP_INH_
#include "Heap.h"

void Heap_Initialize(Heap*);
void Heap_Insert(Heap *,char *,float);
HeapNode * Heap_Update(HeapNode *,char *,float,float,int *);
void Heap_Delete(HeapNode *);
int Heap_Top(Heap *,float);
void Heap_Bye(Heap *);
#endif