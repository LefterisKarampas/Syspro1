#ifndef _CDR_NODE_INH_
#define _CDR_NODE_INH_
#include "CDR_Node.h"
#include "Caller_List.h"

int CDR_Initialize(CDR_Node **);
CDR_Node * CDR_Insert(CDR_Node *,char *,Info *);
float CDR_Remove(CDR_Node *,char *,int,CDR_Node **);
void CDR_Delete(CDR_Node *,int);
void CDR_Copy(CDR_Info *,CDR_Info *);
void CDR_Find(CDR_Node *,Time,Date,Time,Date,int);
int CDR_Indist(CDR_Node *,Caller_List *);
void CDR_TopDest(CDR_Node *,Caller_List *,int *);
void CDR_PrintAll(CDR_Node *);
void CDR_Dump(CDR_Node *,char *,int,FILE *);
#endif