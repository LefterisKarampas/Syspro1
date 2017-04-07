#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Hash.h"
#include "BucketInterface.h"



unsigned long Hash_Function(char *str) // djb2 by Dan Bernstein 
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++) != '\0'){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}



void HT_Initialize(HT **Hash,int buckets,int size_bucket){					//Create Hash_Table
	*Hash = malloc(sizeof(HT));
	(*Hash)->Hash_Table = malloc(sizeof(hash_buckets)*buckets);				//Allocate a table with pointer to buckets
	int i;
	for(i =0;i<buckets;i++){
		(*Hash)->Hash_Table[i].FirstBucket = NULL;							//Initialize pointer to first_bucket with NULL
		(*Hash)->Hash_Table[i].LastBucket = NULL;							//Initialize pointer to last_bucket with NULL
	}
	(*Hash)->buckets = buckets;	
}



float HT_Insert(HT Hash,char * number,char * number2,Info *info,int flag){		//Insert a new CDR in hash_table
	unsigned long int bucket;
	float bill = -1.0;
	int x;
	bucket = Hash_Function(number) % Hash.buckets;									//Find that number where belongs
	if(Hash.Hash_Table[bucket].FirstBucket == NULL){								//If that bucket is empty 
		Hash.Hash_Table[bucket].FirstBucket = malloc(sizeof(Bucket));				//Create it 
		x = Bucket_Initialize(Hash.Hash_Table[bucket].FirstBucket);	//Initialize it
		if(x == -1){
			return -3.0;
		}
		Hash.Hash_Table[bucket].LastBucket = Hash.Hash_Table[bucket].FirstBucket;	//Last_bucket pointer is the same with the first
	}
	bill = Bucket_Insert(Hash.Hash_Table[bucket].FirstBucket,number,number2,info,&(Hash.Hash_Table[bucket].LastBucket),flag);	//Now we can insert the new CDR
	return bill;
}



float HT_Remove(HT Hash,char * number,char * cdr_id,int flag){				//Delete the cdr_id which belongs to a caller if it exists
	unsigned long int bucket;
	float bill = -2.0;
	bucket = Hash_Function(number) % Hash.buckets;							//Find that caller's number where belongs
	if(Hash.Hash_Table[bucket].FirstBucket == NULL){						//If bucket not exists, there is not any caller with that number
		printf("DError: There is not costumer with number: %s\n",number);
		return bill;
	}
	bill = Bucket_Remove(Hash.Hash_Table[bucket].FirstBucket,number,cdr_id,flag,&(Hash.Hash_Table[bucket].LastBucket));
	/*if(bill == -1.0){												//Code for deleting a bucket if it was empty after delete a cdr
		Bucket_Delete(Hash.Hash_Table[bucket].FirstBucket,flag);	//But piazza's guidance is to not delete a caller if has not calls!!
		free(Hash.Hash_Table[bucket].FirstBucket);
		Hash.Hash_Table[bucket].FirstBucket = NULL;
	}*/
	if(bill == -2.0){															//We don't find any caller with that number in bucket
		printf("DError: There is not costumer with number: %s\n",number);		//print message to inform us for that 
	}
	return bill;
}




void HT_Find(HT Hash,char * number,Time time1,Date date1,Time time2,Date date2,int x,int flag){		//Find/Lookup 
	unsigned long int bucket;
	bucket = Hash_Function(number) % Hash.buckets;											//Find that caller's number where belongs
	if(Hash.Hash_Table[bucket].FirstBucket != NULL){												//If bucket exists
		Bucket_Find(Hash.Hash_Table[bucket].FirstBucket,number,time1,date1,time2,date2,x,flag);		//Continue search in bucket
	}
}



void HT_Delete(HT *Hash,int flag){										//Delete Hash_Table
	int i;
	for(i=0;i<Hash->buckets;i++){										//For each bucket in hash_table
		if(Hash->Hash_Table[i].FirstBucket != NULL){					//which exists
			Bucket_Delete(Hash->Hash_Table[i].FirstBucket,flag);		//delete this bucket
			free(Hash->Hash_Table[i].FirstBucket);						//free the pointer to bucket				
		}
	}
	free(Hash->Hash_Table);												//free the whole hash_table
}



int HT_Indist(HT Hash,char *number,Caller_List * List,int flag){
	unsigned long int bucket;
	int x = 0;
	bucket = Hash_Function(number) % Hash.buckets;								//Find that caller's number where belongs
	if(Hash.Hash_Table[bucket].FirstBucket != NULL){							//If bucket exists
		x = Bucket_Indist(Hash.Hash_Table[bucket].FirstBucket,number,List,flag);	//Continue search in bucket
	}
	if(x == -1){
		return -1;
	}
	return 0;
}




void HT_TopDest(HT Hash,char * caller,Caller_List *List,int *max){
	unsigned long int bucket;
	bucket = Hash_Function(caller) % Hash.buckets;								//Find that caller's number where belongs
	if(Hash.Hash_Table[bucket].FirstBucket == NULL){							//If bucket exists
		printf("There is not costumer with number: %s\n",caller);
	}
	else{
		Bucket_TopDest(Hash.Hash_Table[bucket].FirstBucket,caller,List,max);	//Continue search in bucket
	}
}



void HT_Print(HT Hash,int flag){											//Print hash_table
	int i;
	for(i =0;i<Hash.buckets;i++){
		if(Hash.Hash_Table[i].FirstBucket == NULL){							//If bucket not exists 
			printf("\tBucket[%d]: NULL\n",i);								//print him as null
		}
		else{
			printf("\tBucket[%d]:\n",i);									//Else if exists
			Bucket_Print(Hash.Hash_Table[i].FirstBucket,flag);				//Continue to print all the CDRs in it
		}
	}
}



void HT_Dump(HT Hash,int flag,char * file){								//Write the hash_table as insert orders in a file
	int i;
	FILE * fp;
	fp = fopen(file,"w+");												//Create a file with that name
	if(fp == NULL){														//If fail to open and create it 
		fprintf(stderr,"Dump_Error: Fail open file %s\n",file);			//Inform us
	}
	else{																//Otherwise all its ok
		for(i =0;i<Hash.buckets;i++){									//For each bucket
			if(Hash.Hash_Table[i].FirstBucket != NULL){					//which exists
				Bucket_Dump(Hash.Hash_Table[i].FirstBucket,flag,fp);	//Insert to that bucket and write all the CDRs in that file
			}
		}
		fclose(fp);														//Close file
	}
}



void HT_Bye(HT *Hash,int flag){											//Remove everything in Hash_Table excpet the hash_table
	int i;																
	for(i=0;i<Hash->buckets;i++){										//For each bucket in hash_Table
		if(Hash->Hash_Table[i].FirstBucket != NULL){					
			Bucket_Delete(Hash->Hash_Table[i].FirstBucket,flag);		//Delete this bucket
			free(Hash->Hash_Table[i].FirstBucket);						//Free the pointer to the first bucket
			Hash->Hash_Table[i].FirstBucket = NULL;						//Pointer have to show to nowhere
			Hash->Hash_Table[i].LastBucket = NULL;						//Pointer have to show to nowhere
		}
	}
}