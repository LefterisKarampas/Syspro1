#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Heap.h"
#include <math.h>
#include "Caller_List.h"

void DemicalToBinary(char **str,int count){			//Function which is returning a binary string for traverse the heap
	int x;											//0 is Left child
	x = (int) ceil(log2(count+1));					//1 is Right child
	if(x <= 1){										//if count == 1 no action is necessary because we must find root
		(*str) = malloc(sizeof(char)*2);
		(*str)[0] = '0';
		(*str)[1] = '\0';
	}
	else{											//Else we have to make the road to the new node
		*str = malloc(sizeof(char)*x);
		int i;
		i = x-2;									//We are making the string(road) from the end to start
		(*str)[x-1] = '\0';							//Put end of string 
		while(count>1){								//Without put the last division in string is useless
			if(count %2 == 0)
				(*str)[i] ='0';						//0 is Left move
			else
				(*str)[i] = '1';					//1 is Right move
			i--;
			count = count / 2;						
		}
	
	}
}

void swap(HeapNode *node,HeapNode *node2){
	char *temp;
	float temp_bill;
	temp = node->number;
	temp_bill = node->bill;

	node->number = node2->number;
	node->bill = node2->bill;

	node2->number = temp;
	node2->bill = temp_bill;
}


void Heap_Initialize(Heap* heap){
	heap->node = NULL;
	heap->count = 0;
	heap->sum = 0;
}


HeapNode * Heap_Update(HeapNode *node,char * number,float new_bill,float bill,int *flag){
	if(node != NULL){
		if(!strcmp(node->number,number)){			//If find the node with caller's number
			node->bill = new_bill;					//Change caller's bill with the new bill
			*flag = 1;								//Update flag ON
			return node;							
		}
		else if(bill <= node->bill){				//If caller's bill is less than this node's bill search more
			node->Left = Heap_Update(node->Left,number,new_bill,bill,flag);				//First search Left
			if((*flag) == 0){															//If nothing found left
				node->Right = Heap_Update(node->Right,number,new_bill,bill,flag);		//Then search right
			}
			else if(node->Left->bill > node->bill){										//else if found him check if we must heapify the node
				swap(node->Left,node);													
				return node;
			}
		}
		if(node->Right != NULL){												//If right_child exists
			if(((*flag) == 1) &&(node->bill < node->Right->bill)){				//And we found him right, check if we must heapify the node
				swap(node->Right,node);
			}
		}
	}
	return node;
}


void Heap_Delete(HeapNode *node){
	if(node->Right != NULL){						//If exists the right child
		Heap_Delete(node->Right);					//First delete it
		free(node->Right);							//Then free the pointer to it
	}
	if(node->Left != NULL){							//If exists the left child
		Heap_Delete(node->Left);					//First delete it
		free(node->Left);							//Then free the pointer to it
	}		
	free(node->number);
}



HeapNode * Heap_Push(HeapNode *node,char *s,char *number,float bill){
	if(node == NULL){															//Create new node							
		int x = strlen(number);
		node = malloc(sizeof(HeapNode));
		if(node == NULL){
			fprintf(stderr,"Fail malloc in Heap_Push\n");
			return node;
		}
		node->number = (char *) malloc(sizeof(char)*(x+1));
		strcpy(node->number,number);
		node->number[x] = '\0';
		node->bill = bill;
		node->Right = NULL;
		node->Left = NULL;
	}
	else{
		if(s[0] == '0'){														//First char in string is 0, must go left
			node->Left = Heap_Push(node->Left,&(s[1]),number,bill);		
			if((node->Left!= NULL) && (node->bill < node->Left->bill)){
				swap(node,node->Left);											//Heapify
			}
		}
		else{
			node->Right = Heap_Push(node->Right,&(s[1]),number,bill);			//Else first char in string is 1, must go right
			if((node->Right != NULL) && (node->bill < node->Right->bill)){
				swap(node,node->Right);											//Heapify
			}
		}
	}
	return node;
}


void Heap_Insert(Heap *heap,char * number,float bill){
	char *s;
	heap->sum += bill;														//Update the overall income
	heap->count++;															//Update the count of nodes
	DemicalToBinary(&s,heap->count);										//Find the road for insertion
	heap->node = Heap_Push(heap->node,s,number,bill);						//Start traversing
	free(s);
}



int Heap_Find(HeapNode *node,float max,float *sum,Caller_List *List){
	if(node != NULL){
		if(max > node->bill){											
			Caller_List *List1;												//List1 is for left child nodes
			Caller_List *List2;												//List2 is for right child nodes
			List1 = malloc(sizeof(Caller_List));							//Create List1
			if(List1 == NULL){				
				fprintf(stderr,"Fail malloc in Heap_Find\n");
				return -1;
			}
			List_Initialize(List1);											//Initialize List1
			List2 = malloc(sizeof(Caller_List));							//Create List2
			if(List2 == NULL){
				fprintf(stderr,"Fail malloc in Heap_Find\n");
				return -1;
			}
			int x;
			List_Initialize(List2);											//Initialize List2
			x = Heap_Find(node->Left,max - node->bill,sum,List1);
			if(x == -1){
				return -1;
			}
			x = Heap_Find(node->Right,max  - node->bill,sum,List2);
			if(x == -1){
				return -1;
			}
			List_MergeSort(List1,List2,List,max - node->bill);				//Return to List the costumers(right-left childs) with biggest bill
																			//who make the sum(max-node)													

			List_Delete(List1);												//Remove List1
			free(List1);
			List_Delete(List2);												//Remove List2
			free(List2);
		}
		List->count++;			
		*sum += node->bill;
		NodeType * temp;
		temp = malloc(sizeof(NodeType));
		if(temp == NULL){
			fprintf(stderr,"Fail malloc in Heap_Find\n");
			return -1;
		}
		temp->Next = List->head;										//Insert to the Lish as head the parent_node costumer
		temp->data.top.bill = node->bill;
		strcpy(temp->data.top.number,node->number);
		temp->data.top.number[14] = '\0';
		List->head = temp;
	}
	return 0;
}



int Heap_Top(Heap *heap,float k){
	float max = heap->sum * k/100;
	Caller_List * List;
	List = malloc(sizeof(Caller_List));			//Create the List which will have sorted the best costumers who make k% of overall income
	if(List == NULL){
		fprintf(stderr,"Fail malloc in Heap_Top\n");
		return -1;
	}
	List_Initialize(List);
	float sum = 0.0;
	printf("Sum: %.2f - Target: %.2f\n",heap->sum,max);
	if(max > 0){
		Heap_Find(heap->node,max,&sum,List);	//Call the Find fucntion to do the whole job 
		int i;
		NodeType *temp;
		temp = List->head;
		float percent;
		for(i=0;i<List->count;i++){				//Print the costumers who make k% of overall income in company
			percent = 100 *temp->data.top.bill/heap->sum;
			printf("Costumer with number: %s has %.3f bill which is %.3f%% of overall in company\n",temp->data.top.number,temp->data.top.bill,percent);
			temp = temp->Next;
		}
		if(List->count == 0){
			printf("No calls found\n");
		}
	}
	else{
		printf("No calls found\n");
	}
	List_Delete(List);
	free(List);
	return 0;
}


void Heap_Bye(Heap *heap){
	if(heap->count >0){
		Heap_Delete(heap->node);
		free(heap->node);
	}
	heap->node = NULL;
	heap->count = 0;
	heap->sum = 0.0;
}