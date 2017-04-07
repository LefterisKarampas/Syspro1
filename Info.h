#ifndef _INFO_H_
#define _INFO_H_


typedef struct date{
	char month[3];
	char day[3];
	char year[5];
}Date;

typedef struct time{
	char mins[3];
	char hours[3];
}Time;

typedef struct info{
    char * cdr_id;
    Date date;
    Time time;
    unsigned int duration;
    int type;
    int tarrif;
    int fault_condition;
}Info;

typedef struct num{
	char code[3];
	char number[10];
}Number;

float Cost[3][3];
#endif