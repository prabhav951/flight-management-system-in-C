//Function defintions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight_DataBase.h"

//Function to intialize a Flight Database
void initializeDataBase(Flight_DataBase *FDBptr)
{
	FDBptr->isSortedOnKey = TRUE;
	FDBptr->listptr = NULL;
}

//Function to create a node for a flight record
//returns pointer to new node
FR_Node *FR_MakeNode(char *flight_name, unsigned int flight_id, unsigned int flight_capacity, Time flight_arrival_time, Time flight_departure_time, Time flight_delay_time, char *flight_class)
{
	FR_Node *nptr = (FR_Node *)malloc(sizeof(FR_Node));

	strcpy(nptr->name, flight_name);
	nptr->id = flight_id;
	nptr->capacity = flight_capacity;
	nptr->arrival_time = flight_arrival_time;
	nptr->departure_time = flight_departure_time;
	nptr->delay_time = flight_delay_time;
	strcpy(nptr->class, flight_class);
	nptr->key = keyCalculator(flight_id, flight_arrival_time);
	nptr->next = NULL;

	return nptr;
}

//Function to calculate key
//returns key
unsigned long int keyCalculator(unsigned int id, Time arrival_time)
{
	unsigned long int key;
	key = id * 10000 + arrival_time.hr * 100 + arrival_time.min;
	return key;
}

//Function to determine whether database is full or not
//returns TRUE or FALSE
boolean isFull(Flight_DataBase *FDBptr)
{
	return (getNumFlights(FDBptr) == DB_SIZE);
}

//Function to determine whether database is empty or not
//returns TRUE or FALSE
boolean isEmpty(Flight_DataBase *FDBptr)
{
	return (FDBptr->listptr == NULL);
}

//Function to determine the number of flights present in a database
//returns the number of flights
unsigned int getNumFlights(Flight_DataBase *FDBptr)
{
	FR_Node *ptr = FDBptr->listptr;
	unsigned int count = 0;

	while (ptr != NULL)
	{
		count++;
		ptr = ptr->next;
	}

	return count;
}

//Helper function for insert()
//Function to insert/update a record with given data in a particular flight database
//returns SUCCESS or FAILURE
status_code insertInSelectedList(Flight_DataBase *FDBptr, char *flight_name, unsigned int flight_id, unsigned int flight_capacity, Time flight_arrival_time, Time flight_departure_time, Time flight_delay_time, char *flight_class)
{
	status_code ret_val = SUCCESS;
	if (FDBptr->isSortedOnKey == FALSE)
	{
		getSortedOnKey(FDBptr);
	}

	FR_Node *lptr = FDBptr->listptr;
	FR_Node *ptr, *prev, *nptr;
	unsigned long int searchKey = keyCalculator(flight_id, flight_arrival_time);

	if (isFull(FDBptr) == TRUE)
	{
		ret_val = FAILURE;
	}
	else if (lptr == NULL)
	{
		lptr = FR_MakeNode(flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
		lptr->next = NULL;
	}
	else
	{
		ptr = lptr;
		prev = NULL;
		while (ptr != NULL && ptr->key <= searchKey)
		{
			prev = ptr;
			ptr = ptr->next;
		}

		if (prev == NULL)
		{
			nptr = FR_MakeNode(flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
			nptr->next = lptr;
			lptr = nptr;
		}
		else if (prev->key == searchKey)
		{
			strcpy(prev->name, flight_name);
			prev->capacity = flight_capacity;
			prev->departure_time = flight_departure_time;
			prev->delay_time = flight_delay_time;
			strcpy(prev->class, flight_class);
		}
		else
		{
			nptr = FR_MakeNode(flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
			nptr->next = ptr;
			prev->next = nptr;
		}
	}

	FDBptr->listptr = lptr;
	return ret_val;
}

//Function to insert/update a record with given data in a flight database, accordingly updating remaining 3 sorted databases
//returns SUCCESS or FAILURE
status_code insert(Flight_DataBase *mFDBptr, char *flight_name, unsigned int flight_id, unsigned int flight_capacity, Time flight_arrival_time, Time flight_departure_time, Time flight_delay_time, char *flight_class, Flight_DataBase *vvFDBptr, Flight_DataBase *vFDBptr, Flight_DataBase *pFDBptr)
{
	status_code sc;
	sc = insertInSelectedList(mFDBptr, flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
	if (sc != FAILURE)
	{
		unsigned long int searchKey = keyCalculator(flight_id, flight_arrival_time);
		if (strcmp(flight_class, "VVIP") == 0)
		{
			insertInSelectedList(vvFDBptr, flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
			deleteInSelectedList_BasedOnKey(vFDBptr, searchKey);
			deleteInSelectedList_BasedOnKey(pFDBptr, searchKey);
		}
		else if (strcmp(flight_class, "VIP") == 0)
		{
			insertInSelectedList(vFDBptr, flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
			deleteInSelectedList_BasedOnKey(vvFDBptr, searchKey);
			deleteInSelectedList_BasedOnKey(pFDBptr, searchKey);
		}
		else
		{
			insertInSelectedList(pFDBptr, flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
			deleteInSelectedList_BasedOnKey(vvFDBptr, searchKey);
			deleteInSelectedList_BasedOnKey(vFDBptr, searchKey);
		}
	}
	return sc;
}

//Helper function for delete()
//Function to delete a record with given name and id of a flight in a particular flight database
//returns SUCCESS or FAILURE
status_code deleteInSelectedList(Flight_DataBase *FDBptr, unsigned int flight_id)
{
	status_code ret_val = SUCCESS;
	if (FDBptr->isSortedOnKey == FALSE)
	{
		getSortedOnKey(FDBptr);
	}

	FR_Node *lptr = FDBptr->listptr;
	FR_Node *ptr, *prev;

	if (isEmpty(FDBptr) == TRUE)
	{
		ret_val = FAILURE;
	}
	else
	{
		ptr = lptr;
		prev = NULL;

		while (ptr != NULL && ptr->id != flight_id)
		{
			prev = ptr;
			ptr = ptr->next;
		}

		if (ptr == NULL)
		{
			ret_val = FAILURE;
		}
		else
		{
			while (ptr != NULL && ptr->id == flight_id)
			{
				if (prev != NULL)
				{
					prev->next = ptr->next;
					free(ptr);
					ptr = prev->next;
				}
				else
				{
					lptr = ptr->next;
					free(ptr);
					ptr = lptr;
				}
			}
		}
	}

	FDBptr->listptr = lptr;
	return ret_val;
}

//Function to delete a record with given name and id of a flight in a flight database, accordingly updating remaining 3 sorted databases
//returns SUCCESS or FAILURE
status_code delete (Flight_DataBase *mFDBptr, unsigned int flight_id, Flight_DataBase *vvFDBptr, Flight_DataBase *vFDBptr, Flight_DataBase *pFDBptr)
{
	status_code sc;
	sc = deleteInSelectedList(mFDBptr, flight_id);
	if (sc == SUCCESS)
	{
		deleteInSelectedList(vvFDBptr, flight_id);
		deleteInSelectedList(vFDBptr, flight_id);
		deleteInSelectedList(pFDBptr, flight_id);
	}
	return sc;
}

//Function to compare the field "key" in two Nodes
//returns TRUE or FALSE
boolean comparator_Key(FR_Node *ptr1, FR_Node *ptr2)
{
	return (ptr1->key < ptr2->key);
}

//Function to compare the field "arrival_time" in two Nodes
//returns TRUE or FALSE
boolean comparator_ArrivalTime(FR_Node *ptr1, FR_Node *ptr2)
{
	boolean ret_val;
	if (ptr1->arrival_time.hr < ptr2->arrival_time.hr)
		ret_val = 1;
	else if (ptr1->arrival_time.hr > ptr2->arrival_time.hr)
		ret_val = 0;
	else
	{
		if (ptr1->arrival_time.min < ptr2->arrival_time.min)
			ret_val = 1;
		else
			ret_val = 0;
	}
	return ret_val;
}

//Function to compare the field "departure_time" in two Nodes
//returns TRUE or FALSE
boolean comparator_DepartureTime(FR_Node *ptr1, FR_Node *ptr2)
{
	boolean ret_val;
	if (ptr1->departure_time.hr < ptr2->departure_time.hr)
		ret_val = TRUE;
	else if (ptr1->departure_time.hr > ptr2->departure_time.hr)
		ret_val = FALSE;
	else
	{
		if (ptr1->departure_time.min < ptr2->departure_time.min)
			ret_val = TRUE;
		else
			ret_val = FALSE;
	}
	return ret_val;
}

//Function to compare the field "departure_time - arrival_time" in two Nodes
//returns TRUE or FALSE
boolean comparator_StayTime(FR_Node *ptr1, FR_Node *ptr2)
{
	boolean ret_val;
	if (ptr1->departure_time.hr - ptr1->arrival_time.hr < ptr2->departure_time.hr - ptr2->arrival_time.hr)
		ret_val = TRUE;
	else if (ptr1->departure_time.hr - ptr1->arrival_time.hr > ptr2->departure_time.hr - ptr2->arrival_time.hr)
		ret_val = FALSE;
	else
	{
		if (ptr1->departure_time.min - ptr1->arrival_time.min < ptr2->departure_time.min - ptr2->arrival_time.min)
			ret_val = TRUE;
		else
			ret_val = FALSE;
	}
	return ret_val;
}

//Function to compare the fields "id" and "name" in two Nodes
//returns TRUE or FALSE
boolean comparator_Id_Name_DelayTime(FR_Node *ptr1, FR_Node *ptr2)
{
	boolean ret_val;
	if (ptr1->id < ptr2->id)
		ret_val = TRUE;
	else if (ptr1->id > ptr2->id)
		ret_val = FALSE;
	else
	{
		if (strcmp(ptr1->name, ptr2->name) < 0)
			ret_val = TRUE;
		else if (strcmp(ptr1->name, ptr2->name) > 0)
			ret_val = FALSE;
		else
		{
			if (ptr1->delay_time.hr < ptr2->delay_time.hr)
				ret_val = TRUE;
			else if (ptr1->delay_time.hr > ptr2->delay_time.hr)
				ret_val = FALSE;
			else
			{
				if (ptr1->delay_time.min < ptr2->delay_time.min)
					ret_val = TRUE;
				else
					ret_val = FALSE;
			}
		}
	}
	return ret_val;
}

//Helper function for MergeSort()
//Function to merge two sorted lists in a sorted fashion
//returns the head of the merged sorted list
FR_Node *Merge(FR_Node *lptr, FR_Node *nptr, boolean (*comp)(FR_Node *, FR_Node *)) //Assuming lptr and nptr each have atleast one node
{
	FR_Node *result, *ptr1, *ptr2, *tail;

	ptr1 = lptr;
	ptr2 = nptr;

	if (comp(ptr1, ptr2))
	{
		result = ptr1;
		ptr1 = ptr1->next;
	}
	else
	{
		result = ptr2;
		ptr2 = ptr2->next;
	}
	tail = result;

	while (ptr1 != NULL && ptr2 != NULL)
	{
		if (comp(ptr1, ptr2))
		{
			tail->next = ptr1;
			tail = tail->next;
			ptr1 = ptr1->next;
		}
		else
		{
			tail->next = ptr2;
			tail = tail->next;
			ptr2 = ptr2->next;
		}
	}

	if (ptr1 != NULL)
	{
		tail->next = ptr1;
	}
	else
	{
		tail->next = ptr2;
	}

	return result;
}

//Helper function for MergeSort()
//Function to split a list into two lists of equal size
//returns the head of the second half of the splitted list
FR_Node *Divide(FR_Node *lptr)
{
	FR_Node *nptr, *fast, *slow;
	//Taking two ptrs, one moves one step ahead, another moves two steps ahead.
	slow = lptr;
	fast = lptr->next->next;
	while (fast != NULL)
	{
		fast = fast->next;
		slow = slow->next;

		if (fast != NULL)
		{
			fast = fast->next;
		}
	}
	nptr = slow->next;
	slow->next = NULL;

	return nptr;
}

//Function to sort a list based on a comparator function
//returns the head of the sorted list
FR_Node *MergeSort(FR_Node *list_ptr, boolean (*comp)(FR_Node *, FR_Node *))
{
	FR_Node *nptr, *lptr;
	lptr = list_ptr;

	if ((lptr != NULL) && (lptr->next != NULL))
	{
		nptr = Divide(lptr);
		lptr = MergeSort(lptr, comp);
		nptr = MergeSort(nptr, comp);
		lptr = Merge(lptr, nptr, comp);
	}

	return lptr;
}

//Function to sort a flight database based on the field "key"
void getSortedOnKey(Flight_DataBase *FDBptr)
{
	FDBptr->listptr = MergeSort(FDBptr->listptr, comparator_Key);
	FDBptr->isSortedOnKey = TRUE;
}

//Function to sort a flight database based on the field "arrival_time"
void getSortedOnArrivalTime(Flight_DataBase *FDBptr)
{
	FDBptr->listptr = MergeSort(FDBptr->listptr, comparator_ArrivalTime);
	FDBptr->isSortedOnKey = FALSE;
}

//Function to sort a flight database based on the field "departure_time"
void getSortedOnDepartureTime(Flight_DataBase *FDBptr)
{
	FDBptr->listptr = MergeSort(FDBptr->listptr, comparator_DepartureTime);
	FDBptr->isSortedOnKey = FALSE;
}

//Function to sort a flight database based on the field "departure_time - arrival_time"
void getSortedOnStayTime(Flight_DataBase *FDBptr)
{
	FDBptr->listptr = MergeSort(FDBptr->listptr, comparator_StayTime);
	FDBptr->isSortedOnKey = FALSE;
}

//Function to find flights having longest stay time
//prints the data of flights having longest stay time
void getFlightWithtLongestStay(Flight_DataBase *FDBptr)
{
	FR_Node *ptr = FDBptr->listptr;
	int timeInMinutes, maxStayTimeInMinutes;

	if (isEmpty(FDBptr) == TRUE)
	{
		printf("No flights scheduled\n");
	}
	else
	{
		maxStayTimeInMinutes = 60 * (ptr->departure_time.hr - ptr->arrival_time.hr) + (ptr->departure_time.min - ptr->arrival_time.min);
		ptr = ptr->next;
		while (ptr != NULL)
		{
			timeInMinutes = 60 * (ptr->departure_time.hr - ptr->arrival_time.hr) + (ptr->departure_time.min - ptr->arrival_time.min);
			if (timeInMinutes > maxStayTimeInMinutes)
			{
				maxStayTimeInMinutes = timeInMinutes;
			}
			ptr = ptr->next;
		}

		ptr = FDBptr->listptr;
		printf("The flight(s) id and arrival time, with longest stay time are : \n");
		while (ptr != NULL)
		{
			timeInMinutes = 60 * (ptr->departure_time.hr - ptr->arrival_time.hr) + (ptr->departure_time.min - ptr->arrival_time.min);
			if (timeInMinutes == maxStayTimeInMinutes)
			{
				printf("%u\t", ptr->id);
				printTime(&(ptr->arrival_time));
				printf("\n");
			}
			ptr = ptr->next;
		}
	}
}

//Helper function for UpdateFlightStatus()
//Function to insert a node at the start of the StatusList
//returns the new head of the StatusList
StatusList_Node *StatusList_InsertAtStart(StatusList_Node *lptr, char *flight_name, unsigned int flight_id, Time flight_arrival_time, Time flight_delay_time, char *flight_status)
{
	StatusList_Node *nptr = (StatusList_Node *)malloc(sizeof(StatusList_Node));
	strcpy(nptr->name, flight_name);
	nptr->id = flight_id;
	nptr->arrival_time = flight_arrival_time;
	nptr->delay_time = flight_delay_time;
	strcpy(nptr->status, flight_status);

	nptr->next = lptr;
	lptr = nptr;
	return lptr;
}

//Helper function for UpdateFlightStatus()
//Function to delete a flight record in a particular database based on the searchKey
//returns SUCCESS or FAILURE
status_code deleteInSelectedList_BasedOnKey(Flight_DataBase *FDBptr, unsigned long int searchKey)
{
	status_code ret_val = SUCCESS;
	if (FDBptr->isSortedOnKey == FALSE)
	{
		getSortedOnKey(FDBptr);
	}

	FR_Node *lptr = FDBptr->listptr;
	FR_Node *ptr, *prev;

	if (isEmpty(FDBptr) == TRUE)
	{
		ret_val = FAILURE;
	}
	else
	{
		ptr = lptr;
		prev = NULL;

		while (ptr != NULL && ptr->key != searchKey)
		{
			prev = ptr;
			ptr = ptr->next;
		}

		if (ptr == NULL)
		{
			ret_val = FAILURE;
		}
		else
		{
			if (prev != NULL)
			{
				prev->next = ptr->next;
				free(ptr);
				ptr = prev->next;
			}
			else
			{
				lptr = ptr->next;
				free(ptr);
				ptr = lptr;
			}
		}
	}

	FDBptr->listptr = lptr;
	return ret_val;
}

//Function to update flight status based on StatusList
//Prints appropriate message for updation of flight
void UpdateFlightStatus(Flight_DataBase *mFDBptr, Flight_DataBase *vvFDBptr, Flight_DataBase *vFDBptr, Flight_DataBase *pFDBptr, StatusList_Node *list)
{
	if (mFDBptr->isSortedOnKey == FALSE)
	{
		getSortedOnKey(mFDBptr);
	}

	unsigned long int searchKey;
	StatusList_Node *sptr = list;
	FR_Node *prev, *mfptr;
	mfptr = mFDBptr->listptr;

	while (sptr != NULL)
	{
		printf("Evaluating query with data : \n");
		printf("Name\tID\tArrival Time\t\tDeparture Time\n");
		printf("%s\t%u\t", sptr->name, sptr->id);
		printTime(&sptr->arrival_time);
		printf("\t\t");
		printTime(&sptr->delay_time);
		printf("\t");

		searchKey = keyCalculator(sptr->id, sptr->arrival_time);
		mfptr = mFDBptr->listptr;
		prev = NULL;
		while (mfptr != NULL && mfptr->key != searchKey)
		{
			prev = mfptr;
			mfptr = mfptr->next;
		}

		if (mfptr == NULL || strcmp(mfptr->name, sptr->name) != 0)
		{
			printf("Record with given details not found");
		}
		else
		{
			if (strcmp(sptr->status, "DELAY") == 0 || strcmp(sptr->status, "ON_TIME") == 0) //If status is "ON_TIME", set the delay time to 00:00 ,
			{																																								//If status is "DELAY", set the delay time according to the passed delay time parameter.
				if (strcmp(sptr->status, "DELAY") == 0)
				{
					mfptr->delay_time = sptr->delay_time;
				}
				else
				{
					mfptr->delay_time.hr = 0;
					mfptr->delay_time.min = 0;
				}

				Flight_DataBase *cFDBptr;
				if (strcmp(mfptr->class, "VVIP") == 0)
				{
					cFDBptr = vvFDBptr;
				}
				else if (strcmp(mfptr->class, "VIP") == 0)
				{
					cFDBptr = vFDBptr;
				}
				else
				{
					cFDBptr = pFDBptr;
				}

				FR_Node *cfptr = cFDBptr->listptr;
				prev = NULL;
				while (cfptr != NULL && cfptr->key != searchKey)
				{
					prev = cfptr;
					cfptr = cfptr->next;
				}

				if (strcmp(sptr->status, "DELAY") == 0)
				{
					cfptr->delay_time = sptr->delay_time;
				}
				else
				{
					cfptr->delay_time.hr = 0;
					cfptr->delay_time.min = 0;
				}
			}
			else if (strcmp(sptr->status, "CANCELLED") == 0) //delete a flight record in main as well as 3 class databases
			{
				if (prev != NULL)
				{
					prev->next = mfptr->next;
					free(mfptr);
					mfptr = prev->next;
				}
				else
				{
					mFDBptr->listptr = (mFDBptr->listptr)->next;
					free(mfptr);
					mfptr = mFDBptr->listptr;
				}

				if (strcmp(mfptr->class, "VVIP") == 0)
				{
					deleteInSelectedList_BasedOnKey(vvFDBptr, searchKey);
				}
				else if (strcmp(mfptr->class, "VIP") == 0)
				{
					deleteInSelectedList_BasedOnKey(vFDBptr, searchKey);
				}
				else
				{
					deleteInSelectedList_BasedOnKey(pFDBptr, searchKey);
				}
			}
			else if (strcmp(sptr->status, "PROMOTED") == 0) //promote a flight record from one class to the next class in the hierarchy
			{
				if (strcmp(mfptr->class, "VVIP") == 0)
				{
					printf("No further promotion available\n");
				}
				else if (strcmp(mfptr->class, "VIP") == 0)
				{
					strcpy(mfptr->class, "VVIP");
					insertInSelectedList(vvFDBptr, mfptr->name, mfptr->id, mfptr->capacity, mfptr->arrival_time, mfptr->departure_time, mfptr->delay_time, "VVIP");
					deleteInSelectedList_BasedOnKey(vFDBptr, searchKey);
				}
				else
				{
					strcpy(mfptr->class, "VIP");
					insertInSelectedList(vFDBptr, mfptr->name, mfptr->id, mfptr->capacity, mfptr->arrival_time, mfptr->departure_time, mfptr->delay_time, "VIP");
					deleteInSelectedList_BasedOnKey(pFDBptr, searchKey);
				}
			}
			else
			{
				printf("Invalid status\n");
			}
		}

		list = list->next;
		free(sptr);
		sptr = list;
	}
}

//Function to remove duplicate entries. Retain the occurrence with minimum delay, in case of duplicates
void listUnique(Flight_DataBase *SpurFDBptr)
{
	SpurFDBptr->listptr = MergeSort(SpurFDBptr->listptr, comparator_Id_Name_DelayTime);
	SpurFDBptr->isSortedOnKey = FALSE;

	FR_Node *prev, *ptr, *lptr;
	lptr = SpurFDBptr->listptr;

	if (lptr != NULL)
	{
		prev = lptr;
		ptr = lptr->next;
		while (ptr != NULL)
		{
			if (prev->id == ptr->id && strcmp(prev->name, ptr->name) == 0)
			{
				prev->next = ptr->next;
				free(ptr);
				ptr = prev->next;
			}
			else
			{
				ptr = ptr->next;
				prev = prev->next;
			}
		}
	}

	SpurFDBptr->listptr = lptr;
}

//Function to free memory of a particular database
void freeFDB(Flight_DataBase *FDBptr)
{
	FDBptr->isSortedOnKey = TRUE;
	FR_Node *lptr, *ptr;

	lptr = FDBptr->listptr;
	ptr = lptr;
	while (lptr != NULL)
	{
		lptr = lptr->next;
		free(ptr);
		ptr = lptr;
	}
	FDBptr->listptr = NULL;
}

// Function to print Time in 24hr format
void printTime(Time *t)
{
	if (t->hr < 10)
		printf("0%hu:", t->hr);
	else
		printf("%hu:", t->hr);

	if (t->min < 10)
		printf("0%hu", t->min);
	else
		printf("%hu", t->min);
}

// Function to print all details of a particular flight database in tabular format
void printFDB(Flight_DataBase *FDBptr)
{
	FR_Node *ptr = FDBptr->listptr;

	if (FDBptr->listptr == NULL)
	{
		printf("No flights scheduled\n");
	}
	else
	{
		printf("Name\tID\tCapacity\tArrival Time\tDeparture Time\tDelay Time\tClass\n");
		while (ptr != NULL)
		{
			printf("%s\t%u\t%u\t\t", ptr->name, ptr->id, ptr->capacity);
			printTime(&ptr->arrival_time);
			printf("\t\t");
			printTime(&ptr->departure_time);
			printf("\t\t");
			printTime(&ptr->delay_time);
			printf("\t\t");
			printf("%s\t", ptr->class);
			// printf("%lu\t", ptr->key);

			printf("\n");
			ptr = ptr->next;
		}
	}
}