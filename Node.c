#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CDR_NodeInterface.h"
#include "Node.h"
#include "Caller_List.h"
#include "Info.h"


int Node_Initialize(Node *node,char *number,int flag){           
    int x = strlen(number);
    int rcode = 0;
    if(flag == 1){                                                          //flag = 0 means Caller Hash_Table
        node->data.x.number = malloc(sizeof(char)*(x+1));
        if(node->data.x.number == NULL){
            fprintf(stderr,"fail malloc in Node Node_Initialize\n");        //If malloc fail,must exiting!
            return -1;
        }
        strcpy(node->data.x.number,number);
        node->data.x.number[x] = '\0';
        node->data.x.bill = 0;                                              //Initialize the new costumer's bill with zero
    }
    else{                                                                   //flag = 1 means Calle Hash_Table
        node->data.number = malloc(sizeof(char)*(x+1));
        if(node->data.number == NULL){
            fprintf(stderr,"fail malloc in Node Node_Initialize\n");        //If malloc fail,must exiting!
            return -1;
        }
        strcpy(node->data.number,number); 
        node->data.number[x] = '\0';
    }
    rcode = CDR_Initialize(&(node->List));                        //Check if CDR_Initialize fails
    if(rcode == -1){
        return -1;
    }
    node->Last = node->List;                                                //Initialize pointer to last CDR_Node with first CDR_Node
    return 0;
}


float Node_Insert(Node *node,char *number2,Info *info,int flag){
	node->Last = CDR_Insert(node->Last,number2,info);                      //Insert a new CDR to List
    if(node->Last == NULL){                                                //If something goes wrong,inform Bucket for that
        return -3.0;
    }
    if(flag == 1){                                                          //flag = 1 means Caller Hash_Table so have to change caller's bill
        float temp;
        temp = node->data.x.bill;                                           
        if((info->fault_condition >= 200) && (info->fault_condition <= 299)){           //If calls terminate normally
            if(info->type != 0)                                                         //And it is not a message
                node->data.x.bill += Cost[info->type][info->tarrif] * info->duration;   //calculate cost in connection with the duration
            else{                                                                       //else if message
                node->data.x.bill += Cost[0][0];                                        //has fixed cost
            }
        }
        return temp;                                                                    //Return the previous cost
    }
    else{
        return 0.0;
    }
}


void Node_Delete(Node *node,int flag){
    CDR_Delete(node->List,flag);                                               //Delete CDR_Node
    free(node->List);                                                          //Free List
    if(flag == 1)                                                              //Flag = 1 means caller Hash_Table
        free(node->data.x.number);
    else
        free(node->data.number);                                                //flag = 2 means calle Hash_Table
}


float Node_Remove(Node *node,char *cdr_id,int flag){
    float x;
    x = CDR_Remove(node->List,cdr_id,flag,&(node->Last));                       //Remove CDR with cdr_id
    if(x >= 0.0){                                                               // if x>=0 means we find it and remove it
        printf("Delete cdr_id: %s successfully!\n",cdr_id);
        return(x);
    }
    else{                                                                       //else we don't find it
        printf("DError: Not found call with cdr_id %s \n",cdr_id);
        return (x);
    }
}

void Node_Find(Node *node,Time time1,Date date1,Time time2,Date date2,int x,int flag){
    CDR_Find(node->List,time1,date1,time2,date2,x);
}


void Node_Copy(Node *node,Node *temp,int flag){
    if(flag == 1){
        int x = strlen(temp->data.x.number);
        node->data.x.number = malloc(sizeof(char)*(x+1));
        strcpy(node->data.x.number,temp->data.x.number);
        node->data.x.number[x] = '\0';
        node->data.x.bill = temp->data.x.bill;
        free(temp->data.x.number);
    }
    else{
        int x = strlen(temp->data.number);
        node->data.number = malloc(sizeof(char)*(x+1));
        strcpy(node->data.number,temp->data.number);
        node->data.number[x] = '\0';
        free(temp->data.number);
    }
    node->List = temp->List;
    node->Last = temp->Last;
    temp->List = NULL;
    temp->Last = NULL;
}



int Node_Indist(Node *node,Caller_List * caller_List){
    return CDR_Indist(node->List,caller_List);
}


void Node_TopDest(Node *node,Caller_List *caller_List,int *max){
    CDR_TopDest(node->List,caller_List,max);
}


void Node_Print(Node node,int flag){
    if(flag == 1){
        printf("\t\tCaller: %s\n",node.data.x.number);
    }
    else{
        printf("\t\tCallee: %s\n",node.data.number);
    }
    CDR_PrintAll(node.List);
    printf("\n");
}


void Node_Dump(Node node,int flag,FILE *fp){
     if(flag == 1){
        CDR_Dump(node.List,node.data.x.number,flag,fp);
    }
    else{
        CDR_Dump(node.List,node.data.number,flag,fp);
    }
}

