#ifndef _NODE_INH_
#define _NODE_INH_
#include "Node.h"
#include "Caller_List.h"

int Node_Initialize(Node *,char *,int flag);
float Node_Insert(Node *,char *,Info *,int flag);
float Node_Remove(Node *,char *,int);
void Node_Delete(Node *,int);
void Node_Copy(Node *node,Node *temp,int flag);
void Node_Find(Node *,Time,Date,Time,Date ,int,int);
int Node_Indist(Node *,Caller_List *);
void Node_TopDest(Node *,Caller_List *,int *);
void Node_Print(Node ,int);
void Node_Dump(Node,int,FILE *);
#endif