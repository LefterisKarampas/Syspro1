#ifndef _CALLER_LIST_H_
#define _CALLER_LIST_H_

typedef struct d{
	int count;
	char code[4];
}Dest;

typedef struct t{
	float bill;
	char number[15];
}Top;

union Data_List{
	Dest dest;				//For TopDest order
	Top top;				//For Top order
	char *number;			//For Indist order
};

typedef struct NodeTag {
    union Data_List data;
    struct NodeTag *Next;
} NodeType;


typedef struct caller_list{
	int count;
	NodeType * head;
}Caller_List;


void List_Initialize(Caller_List *);
int List_InsertFirstNode(NodeType **,char *);
void List_Remove(NodeType *);
void List_Delete(Caller_List *);
int List_Search(NodeType *,char *);
void List_Concat(Caller_List *,Caller_List *);
int List_Insert(NodeType **,char *,int *);
void List_MergeSort(Caller_List *,Caller_List *,Caller_List *,float);

#endif