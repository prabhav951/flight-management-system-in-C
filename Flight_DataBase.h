//Declaration of structure Flight_DataBase and approriate functions
#ifndef FLIGHT_DATABASE_H
#define FLIGHT_DATABASE_H

#define DB_SIZE 10
#define NAME_LEN 50
#define CLASS_LEN 6
#define STATUS_LEN 10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum
{
	FAILURE,
	SUCCESS
} status_code;
typedef enum
{
	FALSE,
	TRUE
} boolean;

typedef struct Time_tag
{
	unsigned short int hr;
	unsigned short int min;
} Time;

typedef struct FR_Node_tag
{
	char name[NAME_LEN];
	unsigned int id;
	unsigned int capacity;
	Time arrival_time;
	Time departure_time;
	Time delay_time;
	char class[CLASS_LEN];
	unsigned long int key;

	struct FR_Node_tag *next;
} FR_Node;

typedef struct Flight_DB_tag
{
	FR_Node *listptr;
	status_code isSortedOnKey;
} Flight_DataBase;

typedef struct StatusList_Node_tag
{
	char name[NAME_LEN];
	unsigned int id;
	Time arrival_time;
	Time delay_time;
	char status[STATUS_LEN];

	struct StatusList_Node_tag *next;
} StatusList_Node;

void initializeDataBase(Flight_DataBase *FDBptr);
FR_Node *FR_MakeNode(char *flight_name, unsigned int flight_id, unsigned int flight_capacity, Time flight_arrival_time, Time flight_departure_time, Time delay_time, char *flight_class);
unsigned long int keyCalculator(unsigned int id, Time arrival_time);
boolean isFull(Flight_DataBase *FDBptr);
boolean isEmpty(Flight_DataBase *FDBptr);
unsigned int getNumFlights(Flight_DataBase *FDBptr);

status_code insertInSelectedList(Flight_DataBase *FDBptr, char *flight_name, unsigned int flight_id, unsigned int flight_capacity, Time flight_arrival_time, Time flight_departure_time, Time flight_delay_time, char *flight_class); //Helper function for insert()
status_code insert(Flight_DataBase *mFDBptr, char *flight_name, unsigned int flight_id, unsigned int flight_capacity, Time flight_arrival_time, Time flight_departure_time, Time flight_delay_time, char *flight_class, Flight_DataBase *vvFDBptr, Flight_DataBase *vFDBptr, Flight_DataBase *pFDBptr);
status_code deleteInSelectedList_BasedOnKey(Flight_DataBase *FDBptr, unsigned long int searchKey); //Helper function for UpdateFlightStatus()
status_code deleteInSelectedList(Flight_DataBase *FDBptr, unsigned int flight_id);								 //Helper function for delete()
status_code delete (Flight_DataBase *FDBptr, unsigned int flight_id, Flight_DataBase *vvFDBptr, Flight_DataBase *vFDBptr, Flight_DataBase *pFDBptr);

boolean comparator_Key(FR_Node *ptr1, FR_Node *ptr2);
boolean comparator_ArrivalTime(FR_Node *ptr1, FR_Node *ptr2);
boolean comparator_DepartureTime(FR_Node *ptr1, FR_Node *ptr2);
boolean comparator_StayTime(FR_Node *ptr1, FR_Node *ptr2);
boolean comparator_Id_Name_DelayTime(FR_Node *ptr1, FR_Node *ptr2);
FR_Node *Merge(FR_Node *lptr, FR_Node *nptr, boolean (*comp)(FR_Node *, FR_Node *)); //Helper function for MergeSort()
FR_Node *Divide(FR_Node *lptr);																											 //Helper function for MergeSort()
FR_Node *MergeSort(FR_Node *list_ptr, boolean (*comp)(FR_Node *, FR_Node *));
void getSortedOnKey(Flight_DataBase *FDBptr);
void getSortedOnArrivalTime(Flight_DataBase *FDBptr);
void getSortedOnDepartureTime(Flight_DataBase *FDBptr);
void getSortedOnStayTime(Flight_DataBase *FDBptr);

void getFlightWithtLongestStay(Flight_DataBase *FDBptr);

StatusList_Node *StatusList_InsertAtStart(StatusList_Node *lptr, char *flight_name, unsigned int flight_id, Time flight_arrival_time, Time flight_delay_time, char *flight_status); //Helper function for UpdateFlightStatus()
void UpdateFlightStatus(Flight_DataBase *mFDBptr, Flight_DataBase *vvFDBptr, Flight_DataBase *vFDBptr, Flight_DataBase *pFDBptr, StatusList_Node *list);

void listUnique(Flight_DataBase *SpurFDBptr);

void freeFDB(Flight_DataBase *FDBptr);
void printTime(Time *t);
void printFDB(Flight_DataBase *FDBptr);

#endif