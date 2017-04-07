#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeapInterface.h"
#include "HashInterface.h"
#include "Operations_Interface.h"
#include <time.h>


int HashTable1Size = -1;
int HashTable2Size = -1;
int BucketSize = -1;

int main(int argc,char **argv){
	char *file;
	file = NULL;
	int i;
	double sstime,endtime;
	sstime = ((double)clock())/CLOCKS_PER_SEC;	//Start Timer

	for(i=1;i<argc;i++){						//Read the program's arguments from command line
		if(!strcmp(argv[i],"-h1")){				// -h1 flag for Hash_Table1 size
			HashTable1Size = atoi(argv[i+1]);	
		}
		else if(!strcmp(argv[i],"-h2")){		// -h2 flag for Hash_Table1 size
			HashTable2Size = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-o")){			// -o flag for file from which read orders (optional)
			int x = strlen(argv[i+1]); 
			file = malloc(sizeof(char)*(x+1));
			if(file == NULL){
				fprintf(stderr,"Error malloc operation file!\n");
				return -2;
			}
			strcpy(file,argv[i+1]);
			file[x] = '\0';
			printf("Input file: %s\n",file);
		}
		else if(!strcmp(argv[i],"-s")){		// -s flag for Bucket size
			BucketSize = atoi(argv[i+1]);
		}
		if(!strcmp(argv[i],"-c")){			//-c flag for config file from which read cost (optional)
			Create_Cost(argv[i+1]);
		}
	}
	if((HashTable1Size <= 0) || (HashTable2Size <= 0) || (BucketSize <= 0)){	//In case there are few arguments to Initialize Hash Tables exit!
		fprintf(stderr,"Error,arguments are not enought or something is zero!\n");				//Error message
		return -1;																				//Return error code & exit
	}
	printf("HashTable1Size: %d\n",HashTable1Size);
	printf("HashTable2Size: %d\n",HashTable2Size);
	printf("BucketSize: %d\n\n",BucketSize);

	HT * Hash_Table1;
	HT * Hash_Table2;
	Heap * heap;
	heap = malloc(sizeof(Heap));
	if(heap == NULL){											//Check succesful allocation
		fprintf(stderr,"Error malloc heap\n");					//Error message
		return -2;												//Return error code & exit
	}
	int numOfBuckets = BucketSize/(sizeof(Bucket)-2*sizeof(Bucket *));		
	if(numOfBuckets == 0)
		numOfBuckets = 1;
	HT_Initialize(&Hash_Table1,HashTable1Size);				//Initialize hash1(caller)
	HT_Initialize(&Hash_Table2,HashTable2Size);				//Initialize hash2(calle)
	Heap_Initialize(heap);													//Initialize heap
	if(file!=NULL){															
		Operation_File(Hash_Table1,Hash_Table2,heap,file);					//Read orders from file first if it exists!		
		free(file);
	}
	Operation_Prompt(Hash_Table1,Hash_Table2,heap);							//Read orders from prompt 
	Delete_Structure(Hash_Table1,Hash_Table2,heap);							//Deallocate all the structures
	free(heap);																
	endtime = ((double)clock())/CLOCKS_PER_SEC;								//Stop timer
	fprintf(stderr,"Execution time: %.4lf\n",sstime-endtime);				//Print execution time	
	printf("Exiting...!\n");												//Exit message
	return 0;
}