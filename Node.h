#ifndef _NODE_H_
#define _NODE_H_
#include "CDR_Node.h"

typedef struct node Node;

typedef struct call{
	char *number;
	float bill;
}caller;

union Data {
	caller x;			//For Calller Hash_Table
	char * number;		//For Calle Hash_Table
};

typedef struct node{
	union Data data;
	CDR_Node *List;
	CDR_Node *Last;
}Node;

#endif