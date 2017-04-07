#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Info.h"
#include "Operations_Interface.h"
#include "HeapInterface.h"
#include "HashInterface.h"
#include "Caller_List.h"



int Parse(HT *Hash_Table1,HT *Hash_Table2,Heap *heap,char *line){
	char * temp;
	temp = strtok(line,"\n");				//Remove the '\n' from string(line)
	temp = strtok(temp, " ");				//Take the first string which is the order
	int return_code =0;
	if(temp!=NULL){							
		if(!strcmp(temp,"insert")){
			char *args;
			args = strtok(NULL," ");							//Take the second string which is the CDR
			return_code = Insert(*Hash_Table1,*Hash_Table2,heap,args);		//Call the Insert function to do the job for you
		}
		else if(!strcmp(temp,"delete")){
			char * cdr_id;
			char * caller;
			caller = strtok(NULL," ");							//Second string is the caller
			cdr_id = strtok(NULL," ");							//Thrid string is the cdr_id
			HT_Remove(*Hash_Table1,caller,cdr_id,1);			//Find and Remove CDR which belongs to caller and has this unique cdr_id
			printf("\n");
			fflush(stdout);
		}
		else if(!strcmp(temp,"find")){
			char *args;
			args = strtok(NULL," ");							//Second string is the caller
			return_code = Find(*Hash_Table1,args,1);							//Find caller and print all CDR which belongs to him if there are exists
		}
		else if(!strcmp(temp,"lookup")){
			char *args;
			args = strtok(NULL," ");							//Second string is the calle
			return_code = Find(*Hash_Table2,args,2);							//Find calle and print all CDR which belongs to him if there are exists
		}
		else if(!strcmp(temp,"indist")){
			return_code = Indist(*Hash_Table1,*Hash_Table2,temp);				//Call Indist function to do the whole job for you
		}
		else if(!strcmp(temp,"topdest")){
			return_code = TopDest(*Hash_Table1);								//Call TopDest function to do the whole job for you
		}
		else if(!strcmp(temp,"top")){
			float k;
			int x;
			k = atof(strtok(NULL," "));							//Convert the second string, which is the percentage of overall income, to float 
			printf("Top %.2f\n",k);
			x = Heap_Top(heap,k);								//Find the best costumers which create the requested percentage of overall income
			if(x == -1){
				return -1;
			}
			printf("\n\n");
			fflush(stdout);
		}
		else if(!strcmp(temp,"bye")){							//Free all the structures
			HT_Bye(Hash_Table1,1);								//Free Hash_Table1
			HT_Bye(Hash_Table2,2);								//Free Hash_Table2
			Heap_Bye(heap);										//Free Heap
		}
		else if(!strcmp(temp,"print")){
			char *args;
			args = strtok(NULL," ");
			printf("Print %s\n",args);
			if(!strcmp(args,"hashtable1")){
				HT_Print(*Hash_Table1,1);						//Print the whole Hash_Table1 in levels
			}
			else{
				HT_Print(*Hash_Table2,2);						//Print the whole Hash_Table1 in levels
			}
			printf("\n\n");
			fflush(stdout);
		}
		else if(!strcmp(temp,"dump")){
			char *args;
			char *file;
			args = strtok(NULL," ");						//Second string is the Hash_Table 
			file = strtok(NULL," ");						//Thrid string is the file
			printf("Dump %s %s\n",args,file);
			if(!strcmp(args,"hashtable1")){
				HT_Dump(*Hash_Table1,1,file);				//Convert the Hash_Table1 to orders(insert) and write them to the file
			}
			else{
				HT_Dump(*Hash_Table2,2,file);				//Convert the Hash_Table1 to orders(insert) and write them to the file
			}
			printf("\n\n");
			fflush(stdout);
		}
	}
	if(return_code == -1){
		fprintf(stderr,"Sorry,Program must terminate!!\n");
		return return_code;
	}
	return 0;
}



void Operation_File(HT *Hash_Table1,HT *Hash_Table2,Heap *heap,char *file){			//Function for read and run the orders from a file
	FILE *fp;
	fp = fopen(file,"r");															//Open the file
	if(fp == NULL){																	//Check if it exists
		fprintf(stderr,"File with name: %s not exists!\n",file);					//Print error message if not exists
	}
	else{
		size_t len =0;
		char *line = NULL;
		while(getline(&line,&len,fp) != EOF){										//Read each line in file
			if(Parse(Hash_Table1,Hash_Table2,heap,line) <0){						//Decode it and run it
				break;
			}
		}
		free(line);																	//Free garbage
		fclose(fp);																	//Close file
	}
}



void Operation_Prompt(HT * Hash_Table1,HT * Hash_Table2,Heap *heap){				//Function for read and run the orders from prompt
	size_t len =0;
	char *line = NULL;
	while(getline(&line,&len,stdin) != EOF){										//Read each line in file
		if(Parse(Hash_Table1,Hash_Table2,heap,line) < 0){							//Decode it and run it
			break;
		}
	}
	free(line);																		//Free garbage
}


void Create_Cost(char *file){
	FILE *fp;
	fp = fopen(file,"r");															//Open config file if exists
	if(fp == NULL){																	//If not put default costs for every type
		fprintf(stderr,"File with name: %s not exists!\n",file);
		Cost[0][0] = 0.1;
		Cost[1][1] = 0.2;
		Cost[1][2] = 0.8;
		Cost[2][1] = 0.4;
		Cost[2][2] = 0.7;
	}
	else{
		size_t len =0;
		char *line = NULL;
		int count = 0;
		int arg1;
		int arg2;
		float arg3;
		while(getline(&line,&len,fp) != EOF){									//Read each line from file
			if(count != 0){														//First line comments,ignore it
				arg1 = atoi(strtok(line,";"));									//Type
				arg2 = atoi(strtok(NULL,";"));									//tarrif
				arg3 = atof(strtok(NULL,";"));									//Cost
				Cost[arg1][arg2] = arg3;										//Initialization cost for this type and tarrif
			}
			count++;															//Count for avoid the first line
		}
		free(line);																//Free garbage
		fclose(fp);																//Close file
	}
}

void Convert_Date(Date *date,char *dat){										//Convert a string to Date(struct)
	strncpy(date->day,dat,2);
	date->day[2] = '\0';
	strncpy(date->month,&(dat[2]),2);
	date->month[2] = '\0';
	strncpy(date->year,&(dat[4]),4);
	date->year[4] = '\0';
}

void Convert_Time(Time *time,char *init_time){									//Convert a string to Time(struct)
	strncpy(time->hours,init_time,2);
	time->hours[2] = '\0';
	strncpy(time->mins,&(init_time[3]),2);
	time->mins[2] = '\0';
}

void Delete_Structure(HT *Hash_Table1,HT *Hash_Table2,Heap *heap){				//Delete all the structures before exit the program
	if(Hash_Table1 != NULL){
		HT_Delete(Hash_Table1,1);												//Delete Hash_Table1												
		free(Hash_Table1);														//Free the Hash_Table1's space
		Hash_Table1 = NULL;
	}
	if(Hash_Table2 != NULL){
		HT_Delete(Hash_Table2,2);												//Delete Hash_Table2
		free(Hash_Table2);														//Free the Hash_Table2's space
		Hash_Table2 = NULL;
	}
	if(heap->count){
		Heap_Delete(heap->node);												//Remove recursively the whole heap
		free(heap->node);														//Remove the root
	}
}

int Insert(HT Hash_Table1,HT Hash_Table2,Heap *heap,char *args){
	Info *info;
	info = malloc(sizeof(Info));												//Allocate space for CDR_info
	if(info == NULL){
		fprintf(stderr,"IError, fail malloc in insert!\n");						//If it fails,print error message
		return -1;																//And Exit
	}
	char *caller;
	char *calle;
	char *date;
	char *init_time;
	char *t;

	//Seperate the fields in CDR

	t = strtok(args,";");														//First string is cdr_id												
	info->cdr_id = malloc(sizeof(char)*(strlen(t)+1));
	if(info->cdr_id == NULL){
		fprintf(stderr,"IError, fail malloc in insert!\n");
		return -1;
	}
	strcpy(info->cdr_id,t);
	caller = strtok(NULL,";");													//Second string is caller
	calle = strtok(NULL,";");													//Third string is calle
	date = strtok(NULL,";");													//Fourth string is date
	init_time = strtok(NULL,";");												//Fifth string is time
	Convert_Date(&(info->date),date);											//Convert date_string to Date(Struct)
	Convert_Time(&(info->time),init_time);										//Convert time_string to Time(struct)
	info->duration = atoi(strtok(NULL,";"));									//Convert duration_string to int
	info->type = atoi(strtok(NULL,";"));										//Convert type_string to int
	info->tarrif = atoi(strtok(NULL,";"));										//Convert tarrif_string to int
	info->fault_condition = atoi(strtok(NULL,";"));								//Convert fault_condition_string to int
	float previous_bill;
	float bill = 0.0;
	if((info->fault_condition >= 200) && (info->fault_condition <= 299)){		//If fault_condtion is X != 2XX, the call has failed!
        if(info->type != 0)
           bill = Cost[info->type][info->tarrif] * info->duration;				//Call's cost depends on duration
        else{
            bill = Cost[0][0];													//Message has fixed cost
        }
    }
    float return_code;
	previous_bill = HT_Insert(Hash_Table1,caller,calle,info,1);					//Insert the new CDR in caller hash_Table
	if(previous_bill == -3.0){
		return -1;
	}
	return_code = HT_Insert(Hash_Table2,calle,caller,info,2);					//Insert the new CDR in calle hash_Table
	if(return_code == -3.0){
		return -1;
	}

	if(bill > 0){																//If the call is successful
		if(previous_bill != 0.0){												//And caller has other calls
			int flag = 0;
			heap->node = Heap_Update(heap->node,caller,previous_bill+bill,previous_bill,&flag);		//Update his/her bill in heap
			if(flag == 1){
				heap->sum += bill;																	//Update the new company's overall income 
			}
		}
		else{																						//Otherwise its first call
			Heap_Insert(heap,caller,bill);															//Insert new node to heap for him/her
		}
	}
	fflush(stdout);
	return 0;
}

int Find(HT Hash_Table,char * args,int operation){							//Find - Lookup order function
	char * arg1;
	arg1 = strtok(NULL," ");
	Time *time1;
	Time *time2;
	Date *date1;
	Date *date2;
	time1 = malloc(sizeof(Time));
	if(time1 == NULL){
		fprintf(stderr,"FError,fail malloc in find!\n");
		return -1;
	}
	time2 = malloc(sizeof(Time));
	if(time2 == NULL){
		fprintf(stderr,"FError,fail malloc in find!\n");
		return -1;
	}
	date1 = malloc(sizeof(Date));
	if(date1 == NULL){
		fprintf(stderr,"FError,fail malloc in find!\n");
		return -1;
	}
	date2 = malloc(sizeof(Date));
	if(date2 == NULL){
		fprintf(stderr,"FError,fail malloc in find!\n");
		return -1;
	}

	int flag;
	if(arg1 == NULL){														//If not exists date and time limit												 
		if(operation == 1)
			printf("Find %s\n",args);
		else
			printf("Lookup %s\n",args);
		flag = 0;
	}	
	else{																	//Otherwise something exists 
		char * arg2;
		char * arg3;
		arg2 = strtok(NULL," ");
		arg3 = strtok(NULL," ");
		if((arg3 == NULL) && (strlen(arg1) > 5)){				//if there are only 2 args for limitation, check if it is date or time limit
			Convert_Date(date1,arg1);							//Convert date_string to Date(struct)
			Convert_Date(date2,arg2);							//Convert date_string to Date(struct)
			if(operation == 1)
				printf("Find %s in %s - %s\n",args,arg1,arg2);
			else
				printf("Lookup %s in %s - %s\n",args,arg1,arg2);
			flag = 2;
		}
		else if(arg3 == NULL){								
			Convert_Time(time1,arg1);							//Convert time_string to Time(struct)
			Convert_Time(time2,arg2);							//Convert time_string to Time(struct)
			if(operation == 1)
				printf("Find %s in %s - %s\n",args,arg1,arg2);
			else
				printf("Lookup %s in %s - %s\n",args,arg1,arg2);
			flag = 1;
			if(strcmp(arg1,arg2) > 0){							//If time_limit1 is bigger than time_limit2 swap them(piazza's guidance)
				Time *temp;
				temp = time1;
				time1 = time2;
				time2 = temp;
			}
		}
		else{												//Otherwise there are 4 limit arguments
			char * arg4;
			arg4 = strtok(NULL," ");
			Convert_Date(date1,arg2);						//Convert date_string to Date(struct)
			Convert_Date(date2,arg4);						//Convert date_string to Date(struct)
			Convert_Time(time1,arg1);						//Convert time_string to Time(struct)
			Convert_Time(time2,arg3);						//Convert time_string to Time(struct)
			if(operation == 1)
				printf("Find %s in %s - %s and %s - %s \n",args,arg1,arg3,arg2,arg4);
			else 
				printf("Find %s in %s - %s and %s - %s \n",args,arg1,arg3,arg2,arg4);
			flag = 3;
			if(strcmp(arg1,arg3) > 0){						//If time_limit1 is bigger than time_limit2 swap them(piazza's guidance)
			Time *temp;
			temp = time1;
			time1 = time2;
			time2 = temp;
			}
		}
	}
	HT_Find(Hash_Table,args,*time1,*date1,*time2,*date2,flag,operation);		//We are ready to start our search 
	free(time1);																//Free space which allocated
	free(time2);																//Free space which allocated
	free(date1);																//Free space which allocated
	free(date2);																//Free space which allocated
	printf("\n\n");
	fflush(stdout);																//Flush stdout file descriptor
	return 0;
}



int Indist(HT Hash_Table1,HT Hash_Table2,char *temp){
	char * caller1;
	char * caller2;
	caller1 = strtok(NULL," ");	
	caller2 = strtok(NULL," ");
	int rcode;
	Caller_List * List1;														//List1 is for caller1
	Caller_List * List2;														//List2 is for caller2
	List1 = malloc(sizeof(Caller_List));
	if(List1 == NULL){
		fprintf(stderr,"InDistError, fail malloc in indist!\n");
		return -1;
	}
	List2 = malloc(sizeof(Caller_List));
	if(List2 == NULL){
		fprintf(stderr,"InDistError, fail malloc in indist!\n");
		return -1;
	}
	List_Initialize(List1);
	List_Initialize(List2);
	

	printf("%s %s %s\n",temp,caller1,caller2);
	rcode = HT_Indist(Hash_Table1,caller1,List1,1);								//Store caller1's outcoming calls in List1 
	if(rcode == -1){
		return -1;
	}
	rcode = HT_Indist(Hash_Table2,caller1,List1,2);								//Store caller1's incoming calls in List1
	if(rcode == -1){
		return -1;
	}
	rcode = HT_Indist(Hash_Table1,caller2,List2,1);								//Store caller2's outcoming calls in List2
	if(rcode == -1){
		return -1;
	} 
	rcode = HT_Indist(Hash_Table2,caller2,List2,2);								//Store caller2's incoming calls in List2
	if(rcode == -1){
		return -1;
	}

	if((List1->count != 0) && (List2->count != 0)){			//If caller1 or caller2 has not communicated with someone don't check anything 
		NodeType *temp1;
		NodeType *temp2;
		NodeType *start;
		int count = 0;
		int min;
		if(List1->count < List2->count){						//Check out which list is smaller
			temp1 = List1->head;
			start = List2->head;
			min = List1->count;
		}
		else{
			temp1 = List2->head;
			start = List1->head;
			min = List2->count;
		}
		char **Array;
		Array = malloc(sizeof(char *)*min);					//Allocate a Array for costumers who communicate both
		if(Array == NULL){
			fprintf(stderr,"InDistError, fail malloc in indist!\n");
			return -1;
		}

		while(temp1 != NULL){								//Find all those who communicate with both with caller1 and caller2
			temp2 = start;
			while(temp2 != NULL){
				int i;
				int flag = 0;
				if(!strcmp(temp1->data.number,temp2->data.number)){		//If a costumer is member in both lists, hold him! 
					for(i=0;i<count;i++){
						if(!strcmp(Array[i],temp1->data.number)){		//If this costumer has already owned in Array, don't insert him twice
							flag = 1;
							break;
						}
					}
					if(flag == 0){
						Array[count] = temp1->data.number;				//Else insert him for first time
						count++;
					}
				}
				temp2 = temp2->Next;
			}
			temp1 = temp1->Next;
		}

		if(count >=2){						//If there are more than 1 people who communicate with both, we have to check their calls
			int i;
			int j;
			int indist = 0;
			Caller_List * List;
			int Valid_caller[count];		//Allocate a table which show us the who has the characteristics we want
			for(i=0;i<count;i++){
				Valid_caller[i] = 1;		//Initialize all those people with valid flag in start
			}
			List = malloc(sizeof(Caller_List)*count);		//Array of List which hold all the calls for each people we are interested in
			if(List == NULL){
				fprintf(stderr,"InDistError, fail malloc in indist!\n");
				return -1;
			}
			for(i=0;i<count;i++){
				List_Initialize(&(List[i]));
				rcode = HT_Indist(Hash_Table2,Array[i],&(List[i]),2);	//Put those calls in his/her list
				if(rcode == -1){
					return -1;
				}
			}
			int flag;
			for(i=0;i<count;i++){								//Time to check out if they have not spoken to each other
				flag = 0;
				if(Valid_caller[i] == 1){
					for(j=0;j<count;j++){
						flag = List_Search(List[i].head,Array[j]);		//Check if Array[i]_costumer has spoken with Array[i]_costumer
						if(flag == 1){									//If there is communication then rejected them
							Valid_caller[i] = 0;
							Valid_caller[j] = 0;
							break;
						}
						flag = List_Search(List[j].head,Array[i]);		//Check if Array[j]_costumer has spoken with Array[j]_costumer
						if(flag == 1){									//If there is communication then rejected them
							Valid_caller[i] = 0;
							Valid_caller[j] = 0;
							break;
						}
					}
					if(flag == 0){
						indist++;
						printf("%d. %s\n",indist,Array[i]);				//If there are not any calls to others for Array[i]_costumer,print him
					}
				}
			}
			if(indist == 0){									//If there is not someone print message
				printf("No Indist Found\n");
			}
			for(i=0;i<count;i++){
				if(List[i].count != 0)
						List_Delete(&(List[i]));				//Free all the lists in array
			}
			free(List);											//Free Array									
		}
		else if(count == 1){						//Othwise we have only one person that communicate with both,he is our man definitely
			printf("1. %s\n",Array[0]);
		}
		else{										//If none commmunicate with both, print message 
			printf("No Indist Found\n");
		}
		free(Array);
	}

	else{
		if(List1->count == 0){
			printf("InDistError: There is not costumer with number %s\n",caller1);
		}
		if(List2->count == 0){
			printf("InDistError: There is not costumer with number %s\n",caller2);
		}
	}
	if(List1->count != 0 ){							//Delete List1
		List_Delete(List1);
	}
	if(List2->count != 0 ){							//Delete List2
		List_Delete(List2);
	}
	free(List1);
	free(List2);
	printf("\n\n");
	fflush(stdout);
	return 0;
}



int TopDest(HT Hash_Table){
	char * caller;
	caller = strtok(NULL," ");
	int max = 0;
	printf("The Top Code for caller with number: %s\n",caller);

	Caller_List *List;										//List to hold all the codes with the number of calls for each code
	List = malloc(sizeof(Caller_List));
	if(List == NULL){
		fprintf(stderr,"TopError,fail malloc in topdest!\n");
		return -1;
	}
	List_Initialize(List);
	HT_TopDest(Hash_Table,caller,List,&max);				//Return the List with codes and the max number of calls for one code
	int i;
	NodeType *temp;
	temp = List->head;
	for(i=0;i<List->count;i++){						//Search in list and print all the codes which have the same number of calls with max
		if(temp->data.dest.count == max){
			printf("Code: %s with %d calls\n",temp->data.dest.code,max);
		}
		temp = temp->Next;
	}
	if(List->count >0){
		List_Delete(List);							//Delete List
	}
	else{
		printf("No calls found!\n");				//If there is not any call,print message
	}
	free(List);
	printf("\n\n");
	fflush(stdout);
	return 0;
}
