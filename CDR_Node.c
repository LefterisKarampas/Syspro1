#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CDR_Node.h"
#include "Caller_List.h"


extern int BucketSize;

int CDR_Initialize(CDR_Node ** node){
	(*node) = malloc(sizeof(CDR_Node));							//Allocate space for CDR_Info
	if(*node == NULL){
		fprintf(stderr,"Fail malloc in CDR_Initialize\n");		//If malloc fail, must exiting!
		return -1;												//Return fail code
	}
	(*node)->count = 0;
	(*node)->next = NULL;
	(*node)->previous = NULL;
	int size;
	size = BucketSize /sizeof(CDR_Info);
	if(size <= 0){
		size = 1;
	}
	(*node)->max = size;
	(*node)->CDR_Table = malloc(sizeof(CDR_Info)*size);	//Allocate the CDR_Table
	if((*node)->CDR_Table == NULL){
		fprintf(stderr,"Fail malloc in CDR_Initialize\n");		//If malloc fail,must exiting!
		return -1;												//Return fail code
	}
	return 0;													//Return everything goes well
}



void CDR_Print(CDR_Info temp,int tab){							//Print CDR		
	int i;
	for(i=0;i<tab;i++){											//Print n(tab times) tabs
		printf("\t");
	}
	printf("%s",temp.number);
	printf(" %s",temp.info->cdr_id);
	printf(" %s-%s-%s",temp.info->date.day,temp.info->date.month,temp.info->date.year);
	printf(" %s:%s",temp.info->time.hours,temp.info->time.mins);
	printf(" %d",temp.info->duration);
	printf(" %d",temp.info->type);
	printf(" %d",temp.info->tarrif);
	printf(" %d\n",temp.info->fault_condition);
}



CDR_Node * CDR_Insert(CDR_Node * node,char *number,Info *info){					//Insert a new CDR 
	if(node->count < node->max){												//If there is space in this CDR_Node's CDR_Table
		int x = strlen(number);													//then insert it here
		node->CDR_Table[node->count].info = info;
		node->CDR_Table[node->count].number = malloc(sizeof(char)*(x+1));
		if(node->CDR_Table[node->count].number == NULL){
			fprintf(stderr,"Fail malloc in CDR_Insert\n");						//Fail malloc, must exiting!
			return NULL;
		}
		strcpy(node->CDR_Table[node->count].number,number);
		node->CDR_Table[node->count].number[x] = '\0';
		(node->count)++;
		return node;															//Return the last CDR_Node for quickly insertion O(1)
	}
	else{																		//Otherwise create a new CDR_Node
		int x;																
		x = CDR_Initialize(&(node->next));
		if(x == -1){
			return NULL;
		}
		node->next->previous = node;											//Connect CDR_Nodes
		return CDR_Insert(node->next,number,info);								//Now we are ready to insert
	}
	return NULL;																//If all goes wrong return NULL
}



void CDR_Copy(CDR_Info *node,CDR_Info *temp){
	int x = strlen(temp->number);
	node->number = malloc(sizeof(char)*(x+1));
	strcpy(node->number,temp->number);
	node->number[x] = '\0';
	node->info = temp->info;
}


void CDR_Delete(CDR_Node *node,int flag){									//Delete CDR_Node
	node->previous = NULL;
	if(node->next != NULL){													//If there is next CDR_Node delete it first
		CDR_Delete(node->next,flag);										
		free(node->next);													//free next CDR_Node
	}
	int i;
	for(i=0;i<node->count;i++){												//For each CDR_Info in this CDR_Table
		if(flag ==2){							//flag = 2 is for Calle Hash_Table(only here delete CDR because is common for 2 Hash_Tables)
			free(node->CDR_Table[i].info->cdr_id);							//free cdr_id
			free(node->CDR_Table[i].info);									//free CDR
		}
		free(node->CDR_Table[i].number);									//free caller/calle number
	}
	free(node->CDR_Table);													//free CDR_Table
}


float CDR_Remove(CDR_Node *node,char * cdr_id,int flag,CDR_Node **last){
	int i;
	CDR_Node * temp;
	for(i=0;i<node->count;i++){												//For each CDR_Info in CDR_Table
		if(!strcmp(node->CDR_Table[i].info->cdr_id,cdr_id)){				//If exists cdr_id, remove it									
			int index;
			index = (*last)->count-1;										
			if((i != index) || (node != (*last))){			//If this CDR_Info which remove is not in the last CDR_Node in the last position
				free(node->CDR_Table[i].number);			//Remove it
				CDR_Copy(&(node->CDR_Table[i]),&((*last)->CDR_Table[index]));	//But come to his position the CDR_Info which is the last 
			}
			free((*last)->CDR_Table[index].number);			//Remove the space which has the last CDR_Info before moving
			((*last)->count)--;								//Decrase the number of CDR_Info exists in last CDR_Node

			if(((*last)->count == 0) && ((*last)->previous)!=NULL){		//If last CDR_Node is empty now,delete it and make last the previous of it
				temp = (*last)->previous;
				free((*last)->CDR_Table);
				(*last)->previous->next = NULL;
				free((*last));
				(*last) = temp;
				return 1.0;
			}
			else if(((*last)->count == 0)){								//If last = first and is empty,inform the Node
				return 0.0;
			}
			return 1.0;
		}
	}
	if(node->next != NULL){										//If there is not this cdr_id here,check to the next CDR_Node if it exists
		return CDR_Remove(node->next,cdr_id,flag,last);
	}
	return -1.0;												//We don't find anything like this cdr_id
}


int Compare_Time(Time time,Time time1){							//Function to compare Time
	int h; 
	h = strcmp(time.hours,time1.hours);
	if(h > 0){													//First check the hour
		return 1;
	}
	else if(h == 0){											//If hour is same to both of them
		if(strcmp(time.mins,time1.mins) >= 0){					//Check the minutes
			return 1;											//Return 1 if time>time1
		}
	}
	return 0;													//Return 0 if time<=time1	
}

		
int Compare_Date(Date date,Date date1){						//Function to compare Time
	int y;
	y = strcmp(date.year,date1.year);
	if(y >0){												//First check the year
		return 1;
	}
	else if(y == 0){										//If year is the same to both of them
		int m;
		m = strcmp(date.month,date1.month);
		if(m > 0){											//Compare the month
			return 1;
		}
		else if(m == 0){									//If month again is same
			if(strcmp(date.day,date1.day) >= 0){			//You have to compare day
				return 1;									//Return 1 if date>dat1
			}
		}
	}
	return 0;												//Return 0 if date<=date1
}


void CDR_Find(CDR_Node *node,Time time1,Date date1,Time time2,Date date2,int flag){
	int i;
	int count = 0;
	CDR_Node * temp;
	temp = node;
	while(temp != NULL){									//For each CDR_Node
		for(i=0;i<temp->count;i++){							//For each CDR_Info in CDR_Table
			if(flag == 0){									//Flag = 0 means no limits
				count++;
				printf("%d. ",count);
				CDR_Print(temp->CDR_Table[i],0);			//Print CDR_Info
			}
			else if(flag == 1){								//Flag =1 means only limits to Time
				if((Compare_Time(temp->CDR_Table[i].info->time,time1)) && (Compare_Time(time2,temp->CDR_Table[i].info->time))){
					count++;
					printf("%d. ",count);
					CDR_Print(temp->CDR_Table[i],0);		//Print CDR_Info
				}
			}
			else if(flag == 2){							//Flag = 2 means only limits to Date
				if((Compare_Date(temp->CDR_Table[i].info->date,date1)) && (Compare_Date(date2,temp->CDR_Table[i].info->date))){
					count++;
					printf("%d. ",count);
					CDR_Print(temp->CDR_Table[i],0);	//Print CDR_Info
				}
			}
			else{										//Compare both time and date
				if((Compare_Time(temp->CDR_Table[i].info->time,time1)) && (Compare_Time(time2,temp->CDR_Table[i].info->time))){
					if((Compare_Date(temp->CDR_Table[i].info->date,date1)) && (Compare_Date(date2,temp->CDR_Table[i].info->date))){
						count++;
						printf("%d. ",count);
						CDR_Print(temp->CDR_Table[i],0);
					}
				}
			}
		}
		temp = temp->next;							//Go to the next CDR_Node
	}
	if(count == 0){
		printf("No CDRs Found!\n");					//If there is any call within the limits
	}
}


int CDR_Indist(CDR_Node *node,Caller_List * caller_List){
	int i;
	CDR_Node *temp;
	temp = node;
	int x;
	while(temp != NULL){
		for(i=0;i<node->count;i++){													//For each CDR_Info in CDR_Table
			(caller_List->count)++;					
			x = List_InsertFirstNode(&(caller_List->head),node->CDR_Table[i].number);	//Insert it to List
			if(x == -1){
				return -1;
			}
		}
		temp = temp->next;															//Go to the next CDR_Node
	}
	return 0;
}



void CDR_TopDest(CDR_Node *node,Caller_List *caller_List,int *max){
 	int i;
 	int x;
	*max = 0;																//Initialize max = 0
	CDR_Node *temp;
	temp = node;
	char code[4];
	while(temp != NULL){													//For each CDR_Node										
	 	for(i=0;i<temp->count;i++){											//For each CDR_Info in CDR_Table
	 		strncpy(code,temp->CDR_Table[i].number,3);						//Copy CDR_Info's code
	 		code[3] = '\0';
	 		x = List_Insert(&(caller_List->head),code,max);					//Insert this code to List
	 		if(x == 1){
	 			(caller_List->count)++;										//if x = 1 means first time see this code
	 		}
	 	}
	 	temp = temp->next;													//Go to next CDR_Node
	}
 }



void CDR_PrintAll(CDR_Node *node){
	int i;
	CDR_Node *temp;
	temp = node;
	while(temp != NULL){													//For each CDR_Node	
		for(i=0;i<temp->count;i++){											//For each CDR_Info in CDR_Table
			CDR_Print(temp->CDR_Table[i],4);								//Print CDR with 4 tabs
		}
		temp = temp->next;													//Go to next CDR_Node
	}
}



void CDR_Dump(CDR_Node *node,char *number,int flag,FILE *fp){
	int i;
	CDR_Node *temp;
	temp = node;
	while(temp != NULL){													//For each CDR_Node
		for(i=0;i<temp->count;i++){											//For each CDR_Info in CDR_Table
			fprintf(fp,"insert %s",temp->CDR_Table[i].info->cdr_id);
			if(flag == 1){													//flag = 1 means Caller Hash_Table
				fprintf(fp,";%s;%s",number,temp->CDR_Table[i].number);
			}
			else{
				fprintf(fp,";%s;%s",temp->CDR_Table[i].number,number);		//flag = 2 means Calle Hash_Table
			}
			fprintf(fp,";%s%s%s",temp->CDR_Table[i].info->date.day,temp->CDR_Table[i].info->date.month,temp->CDR_Table[i].info->date.year);
			fprintf(fp,";%s:%s",temp->CDR_Table[i].info->time.hours,temp->CDR_Table[i].info->time.mins);
			fprintf(fp,";%d",temp->CDR_Table[i].info->duration);
			fprintf(fp,";%d",temp->CDR_Table[i].info->type);
			fprintf(fp,";%d",temp->CDR_Table[i].info->tarrif);
			fprintf(fp,";%d\n",temp->CDR_Table[i].info->fault_condition);
		}
		temp = temp->next;													//Go to next CDR_Node
	}
}