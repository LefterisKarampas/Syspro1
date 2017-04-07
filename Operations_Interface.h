#ifndef _OP_H_
#define _OP_H_

#include "Hash.h"
#include "Heap.h"

void Create_Cost(char *);
void Delete_Structure(HT *,HT *,Heap *);
int Parse(HT *,HT *,Heap *,char *);
void Operation_File(HT *,HT *,Heap *,char *);
void Operation_Prompt(HT *,HT *,Heap *);

int Insert(HT,HT ,Heap *,char *);
int Find(HT,char *,int);
int Indist(HT,HT,char *);
int TopDest(HT);
#endif