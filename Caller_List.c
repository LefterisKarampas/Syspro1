#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Caller_List.h"


void List_Initialize(Caller_List *List){
	List->count = 0;
	List->head = NULL;
}


int List_InsertFirstNode(NodeType **node,char *number){					//List Insert for Indist Order
	NodeType *N, *temp;
    N=(NodeType *)malloc(sizeof(NodeType));
    if(N == NULL){
    	fprintf(stderr,"Fail malloc in List_InsertFirstNode\n");		//If malloc fails,must exiting!
  		return -1;
    }
    N->data.number = number;
    N->Next=NULL;

    if (*node == NULL){								//If head of list is NULL
    	*node=N;									//Insert new node to head
    }
    else {											//Otherwise head is not NULL
        temp=*node;									
        N->Next = temp;								//Previous head connect as next to the new head
        *node=N;									//Insert new node to head
    }
    return 0;
}


int List_Search(NodeType *node,char *number){
	if(node == NULL){
		return 0;
	}
	if(!strcmp(node->data.number,number))			//if find the number in List
		return 1;									//return 1 for success
	else
		return List_Search(node->Next,number);		//else go to the next node for searching
}



void List_Remove(NodeType *node){
	if(node->Next != NULL){
		List_Remove(node->Next);				//Remove the next_node first
		free(node->Next);						//free the pointer to next node
	}
}


void List_Delete(Caller_List *List){
	if(List->head != NULL){
		List_Remove(List->head);				//Remove the head of List
		free(List->head);						//Free the poninter to head of List
	}
}


void List_Concat(Caller_List *List1,Caller_List *List2){
	if(List2->count > 0){
		if(List1->count >0){					//If List1 is not empty
			NodeType *temp1 = List1->head;
			while(temp1->Next != NULL){			//While find the last node in List1
				temp1 = temp1->Next;			//Go to next
			}
			temp1->Next = List2->head;			//The last node in List1 connected with the head of List2
			List1->count += List2->count;		//Increase the count of elements in List1
		}
		else{									//Else if Lists1 is empty
			List1->head = List2->head;		
			List2->head = NULL;
			List1->count += List2->count;		//Increase the count of elements in List1
			List2->count = 0;
		}
	}
}


int List_Insert(NodeType **head,char *code,int *max){			//List Insert for TopDest order
 	int x = 0;
 	int flag = 1;
	if(*head == NULL){											//If head of List is NULL						
		NodeType *N;
		N=(NodeType *)malloc(sizeof(NodeType));					//Allocate a new node
		N->Next=NULL;											//Initialize it
		N->data.dest.count = 1;
		x = 1;
		strcpy(N->data.dest.code,code);
		N->data.dest.code[3] = '\0';
    	*head=N;												//Make this node the head of List
    } 
	else{														//Otherwise
    	NodeType *temp;
    	temp=*head;
        while (temp->Next != NULL){								//Check if this code is in List already
        	if(!strcmp(temp->data.dest.code,code)){				//If it is true
        		temp->data.dest.count += 1;
        		x = temp->data.dest.count;						//Increase the counter_call by 1
        		flag = 0;
        		break;
        	}
        	temp = temp->Next;									//Else go to the next node 
        }
    	if(flag == 1){											//If we don't find this code in List
    		if(!strcmp(temp->data.dest.code,code)){				//Check if it is in the last node of List
    			temp->data.dest.count += 1;
	        	x = temp->data.dest.count;					
	        	flag = 0;
    		}
    		else{												//If it is not in the last
		    	NodeType *N;									//Create a new node for this code
				N=(NodeType *)malloc(sizeof(NodeType));
				N->Next=NULL;
				N->data.dest.count = 1;							//Initialize it
				x = 1;
				strcpy(N->data.dest.code,code);
				N->data.dest.code[3] = '\0'; 
		    	temp->Next=N;									//Make this node the last of List
		    }
	    }
	}
    if(*max < x){
    	*max = x;												//After the insertion code_calls is bigger than max then make that max
    }
    return flag;												//Return flag to inform for new node
}



void List_MergeSort(Caller_List * List1,Caller_List * List2,Caller_List *List,float max){
	NodeType *last;
	NodeType *temp1;
	NodeType *temp2;
	float count = 0.0;
	temp1 = List1->head;
	temp2 = List2->head;
	while(count < max){													//Until List sum is less than max insert the biggest to List
		if((List1->count > 0) && (List2->count > 0)){
			if(temp1->data.top.bill > temp2->data.top.bill){			
				List1->head = temp1->Next;								//Remove the first node of List1
				temp1->Next = NULL;										//For this node remove the connection to next_node
				List1->count--;											//Decrease the count of List1
				if(List->count == 0){									//If new List is empty
					List->head = temp1;									//Insert that node as head
				}
				else{
					last->Next = temp1;									//Else insert it as last node
				}
				last = temp1;											//Make the last pointer to show at new last node
				temp1 = List1->head;									//Make temp1 pointer show at new first node of List1
			}
			else{
				List2->head = temp2->Next;								//Remove the first node of List2
				temp2->Next = NULL;										//For this node remove the connection to next_node
				List2->count--;											//Decrease the count of List2
				if(List->count == 0){									///If new List is empty
					List->head = temp2;									//Insert that node as head
				}
				else{
					last->Next = temp2;									//Else insert it as last node	
				}
				last = temp2;											//Make the last pointer to show at new last node
				temp2 = List2->head;									//Make temp2 pointer show at new first node of List1
			}
		}
		else if(List1->count > 0){										//If List2 is empty
			List1->head = temp1->Next;									//Do the same only for List1
			temp1->Next = NULL;
			List1->count--;
			if(List->count == 0){
				List->head = temp1;
			}
			else{
				last->Next = temp1;
			}
			last = temp1;
			temp1 = List1->head;
		}
		else if(List2->count > 0){										//Else if List1 is empty
			List2->head = temp2->Next;									//Do the same only for List2
			temp2->Next = NULL;
			List2->count--;
			if(List->count == 0){
				List->head = temp2;
			}
			else{
				last->Next = temp2;
			}
			last = temp2;
			temp2 = List2->head;
		}
		else{														//If there is not other nodes both of Lists
			break;													//Get out of loop
		}
		List->count++;
		count += last->data.top.bill;
	}
}