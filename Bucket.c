#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NodeInterface.h"
#include "Bucket.h"
#include "Caller_List.h"

extern int BucketSize;

int Bucket_Initialize(Bucket *bucket){			//Create a new bucket
	bucket->count = 0;									//Initialize the counter_entries with zero
	int size;
	size = BucketSize / sizeof(Node);
	if(size <= 0){
		size = 1;
	}
	bucket->max_entry = size;							//Initialize the max_entries with the max
	bucket->NodeArray = malloc(sizeof(Node)*size);		//Create the Array which has all the Nodes
	bucket->Next = NULL;								//Pointer to the Next_Bucket
	if(bucket->NodeArray == NULL){
		fprintf(stderr,"fail malloc in Bucket_Initialize\n");
		return -1;
	}
	bucket->Previous = NULL;							//Pointer to the Last_Bucket (we want it in case to deletion a bucket if it is free)
														//Piazza's guidance say not delete a caller if has not calls!!
	return 0;
}


float Bucket_Insert(Bucket *bucket,char *number,char *number2,Info * info,Bucket **Last,int flag){
	float x;
	int i;
	int rcode = 0;
	for(i=0;i<bucket->count;i++){										//For each node in bucket
		int k;
		if(flag == 1)
			k = strcmp(bucket->NodeArray[i].data.x.number,number);		//Seach if there is caller with that number (caller Bucket)
		else
			k = strcmp(bucket->NodeArray[i].data.number,number);		//Seach if there is caller with that number (calle Bucket)
		if(!k){
			x = Node_Insert(&(bucket->NodeArray[i]),number2,info,flag);	//If exists then insert a new CDR to his/her calls
			return x;
		}
	}
	if(bucket->count < bucket->max_entry){								//If we don't have any other bucket and have space to it insert a new caller 
		rcode = Node_Initialize(&(bucket->NodeArray[bucket->count]),number,flag);
		if(rcode == -1){
			return -3.0;												//There is fail in Node_Initialize
		}
		x = Node_Insert(&(bucket->NodeArray[bucket->count]),number2,info,flag);
		(bucket->count)++;												
		(*Last) = bucket;												//Return pointer to last bucket in Hash_Table for quick deletion
	}
	else if(bucket->Next == NULL){										//If we search all buckets not find something and not have space in this bucket	
		bucket->Next = malloc(sizeof(Bucket));							//Create new bucket
		if(bucket->Next == NULL){
			fprintf(stderr,"fail malloc in Bucket_Insert\n");
			return -3.0;
		} 
		Bucket_Initialize(bucket->Next);				//Initialize it
		bucket->Next->Previous = bucket;								//Connect it with previous bucket 
		x = Bucket_Insert(bucket->Next,number,number2,info,Last,flag);	//Now we can insert the new caller
	}
	else{																//if we search a bucket and not find someone with this number
		x = Bucket_Insert(bucket->Next,number,number2,info,Last,flag);	//We have to search the next bucket if exists
	}
	return x;															//Return the bill which has this caller
}

float Bucket_Remove(Bucket *bucket,char *number,char * cdr_id,int flag,Bucket ** Last){
	int i;
	float x;
	for(i=0;i<bucket->count;i++){									//For each Node in bucket search if there is caller with this number
		if(!strcmp(bucket->NodeArray[i].data.x.number,number)){		//If exists
			x = Node_Remove(&(bucket->NodeArray[i]),cdr_id,flag);	//Find and remove the CDR with cdr_id

			//Code for delete a caller who has not any calls after deleting his last call 
			/*
			int index = (*Last)->count-1;
			if(x == 0.0 && ((i != index) || (bucket != (*Last)))){
				Node_Delete(&(bucket->NodeArray[i]),flag);
				Node_Copy(&(bucket->NodeArray[i]),&((*Last)->NodeArray[index]),flag);
				((*Last)->count)--;
			}
			else if(x == 0.0){
				Node_Delete(&(bucket->NodeArray[i]),flag);
				((*Last)->count)--;
			}
			if(((*Last)->count == 0) &&((*Last)->Previous != NULL)){
				temp = (*Last)->Previous;
				(*Last)->Previous->Next = NULL;
				free((*Last)->NodeArray);
				free(*Last);
				(*Last) = temp;
				return x;
			}
			else if((*Last)->count == 0){
				return -1.0;
			}*/
			return x;
		}
	}
	if(bucket->Next != NULL)											//If we don't find him/here here and exists other bucket
		return Bucket_Remove(bucket->Next,number,cdr_id,flag,Last);		//We have to search the other bucket
	else
		return -2.0;													//Else we don't find anyone with this number
}

void Bucket_Find(Bucket *bucket,char *number,Time time1,Date date1,Time time2,Date date2,int y,int flag){
	int i;
	int x = 0;
	for(i=0;i<bucket->count;i++){				//For each node in bucket
		int k;
		if(flag == 1)
			k = strcmp(bucket->NodeArray[i].data.x.number,number);		//Caller's number exists in this bucket, turn flag k on
		else
			k = strcmp(bucket->NodeArray[i].data.number,number);		//Calle's number exists in this bucket, turn flag k on
		if(!k){
			Node_Find(&(bucket->NodeArray[i]),time1,date1,time2,date2,y,flag);	//If exists this number in bucket then continue searching his/her CDRs
			x = 1;
			break;
		}
	}
	if(x == 0){																			//If not exists in this bucket
		if(bucket->Next != NULL)														//And there is next_bucket
			return Bucket_Find(bucket->Next,number,time1,date1,time2,date2,y,flag);		//Search him/her in the next_bucket
		else
			printf("There is not costumer with number: %s\n",number);		//Else print message to inform us that there is not this number
	}
}

void Bucket_Delete(Bucket *bucket,int flag){
	int i;
	if(bucket->Next != NULL){					//If exists next_bucket
		Bucket_Delete(bucket->Next,flag);		//Delete it
		free(bucket->Next);						//free the pointer to the next_bucket
	}
	for(i=0;i<bucket->count;i++){						//For each node in bucket
		Node_Delete(&(bucket->NodeArray[i]),flag);		//Delete the node
	}
	free(bucket->NodeArray);							//Free the Array of Nodes
}



int Bucket_Indist(Bucket *bucket,char * number,Caller_List *NodeArray,int flag){
	int i;
	int x = 0;
	int rcode = 0;
	for(i=0;i<bucket->count;i++){										//For each node in bucket
		int k;
		if(flag == 1)
			k = strcmp(bucket->NodeArray[i].data.x.number,number);		//Caller's number exists in this bucket, turn flag k on
		else
			k = strcmp(bucket->NodeArray[i].data.number,number);		//Calle's number exists in this bucket, turn flag k on
		if(!k){
			rcode = Node_Indist(&(bucket->NodeArray[i]),NodeArray);				//If exists this number in bucket then continue searching his/her CDRs
			if(rcode == -1){
				return -1;
			}
			x= 1;
			break;
		}
	}
	if(x == 0){
		if(bucket->Next != NULL){										//Search for number to the next_bucket if exists
			Bucket_Indist(bucket->Next,number,NodeArray,flag);
		}
	}
	return 0;
}

void Bucket_TopDest(Bucket * bucket,char *caller,Caller_List *NodeArray,int *max){
	int i;
	int flag = 0;
	for(i=0;i<bucket->count;i++){										//For each node in bucket
		if(!strcmp(bucket->NodeArray[i].data.x.number,caller)){			//If caller's number exists in this bucket
			Node_TopDest(&(bucket->NodeArray[i]),NodeArray,max);		//Continue searcing for topdest in his/her CDRs
			flag = 1;										
			break;									
		}
	}
	if(flag == 0){														//If not find this number in this bucket
		if(bucket->Next != NULL){										//And exists the next_bucket
			Bucket_TopDest(bucket->Next,caller,NodeArray,max);			//Search for the number here
		}
		else{
			printf("There is not costumer with number: %s\n",caller);	//Else there is not costumer with this number
		}
	}
}

void Bucket_Print(Bucket *bucket,int flag){							
	int i;
	Bucket *temp;
	temp = bucket;
	while(temp != NULL){												//For each bucket
		for(i=0;i<temp->count;i++){										//For each node in bucket
			Node_Print(temp->NodeArray[i],flag);						//Print CDRs in this node
		}
		temp = temp->Next;												//Go to the next_bucket
	}
}

void Bucket_Dump(Bucket *bucket,int flag,FILE *fp){
	int i;
	Bucket *temp;
	temp = bucket;	
	while(temp != NULL){												//For each bucket
		for(i=0;i<temp->count;i++){										//For each node in bucket
			Node_Dump(temp->NodeArray[i],flag,fp);					//Write CDRs in this node to the file as insert order
		}
		temp = temp->Next;												//Go to the next_bucket
	}
}