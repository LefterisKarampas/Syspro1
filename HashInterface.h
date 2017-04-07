#ifndef _HASH_INH_
#define _HASH_INH_

#include "Hash.h"
#include "Caller_List.h"
//#include "Node.h"

void HT_Initialize(HT **,int);
float HT_Insert(HT,char *,char * ,Info *,int);
float HT_Remove(HT,char *,char *,int);
void HT_Delete(HT *,int);
int HT_Indist(HT,char *,Caller_List *,int);
void HT_Find(HT,char *,Time,Date ,Time,Date,int,int);
void HT_TopDest(HT,char *,Caller_List *,int *);
void HT_Print(HT ,int);
void HT_Dump(HT,int,char *);
void HT_Bye(HT *,int);
#endif