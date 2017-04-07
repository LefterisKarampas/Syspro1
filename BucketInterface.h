#ifndef _BUCKET_INH_
#define _BUCKET_INH_
#include "Bucket.h"
#include "Caller_List.h"

int Bucket_Initialize(Bucket *);
float Bucket_Insert(Bucket *,char *,char *,Info *,Bucket **,int);
float Bucket_Remove(Bucket *,char *,char *,int,Bucket **);
void Bucket_Delete(Bucket *,int);
int Bucket_Indist(Bucket *,char *,Caller_List *,int);
void Bucket_Find(Bucket *,char *,Time,Date ,Time,Date,int,int);
void Bucket_TopDest(Bucket *,char *,Caller_List *,int *);
void Bucket_Print(Bucket *,int );
void Bucket_Dump(Bucket *,int,FILE *);
#endif
