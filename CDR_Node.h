#ifndef _CDR_NODE_H_
#define _CDR_NODE_H_
#include "Info.h"


typedef struct cdr_info{
	char *number;
	Info * info;
}CDR_Info;

typedef struct cdr_node CDR_Node;

typedef struct cdr_node{
	CDR_Info * CDR_Table;
	int count;
	int max;
	CDR_Node * next;
	CDR_Node * previous;		
}CDR_Node;

#endif