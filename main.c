//Driver program to implement Flight Management System using Singly Linked Lists
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight_DataBase.h"

int main()
{
	Flight_DataBase mainFDB, vvipFDB, vipFDB, publicFDB;
	StatusList_Node *list2;

	char *flight_name = (char *)malloc(sizeof(char) * NAME_LEN);
	unsigned int flight_id;
	unsigned int flight_capacity;
	Time flight_arrival_time;
	Time flight_departure_time;
	Time flight_delay_time;
	char *flight_class = (char *)malloc(sizeof(char) * CLASS_LEN);
	char *flight_status = (char *)malloc(sizeof(char) * STATUS_LEN);
	unsigned long int key;
	int i, control_line, numOfRecordsToBeUpdated, numRec;
	status_code sc;

	initializeDataBase(&mainFDB);
	initializeDataBase(&vvipFDB);
	initializeDataBase(&vipFDB);
	initializeDataBase(&publicFDB);
	list2 = NULL;

	control_line = 1;
	while (control_line != 0)
	{
		printf("\n_______________________________________________________\n");
		printf("0   -  Exit terminal\n");
		printf("1   -  Print\n");
		printf("2   -  Insert\n");
		printf("3   -  Delete Records of a particular flight id\n");
		printf("4   -  Number of Flights\n");
		printf("5   -  Check whether list is full or not\n");
		printf("6   -  Check whether list is empty or not\n");
		printf("7   -  Sort based on key\n");
		printf("8   -  Sort based on arrival time\n");
		printf("9   -  Sort based on departure time\n");
		printf("10  -  Sort based on stay time\n");
		printf("11  -  Get the Flight with the longest stay\n");
		printf("12   - Update Flight Status\n");
		printf("13   - List Unique (For a spurious list)\n");

		printf("\nEnter the number beside the function you want to execute : ");
		scanf("%d", &control_line);
		printf("\n");

		switch (control_line)
		{
			case 0: //Exit
			{
				printf("User Interface exited\n");
				break;
			}

			case 1: //Print
			{
				printf("ALL FLIGHTS : \n");
				printFDB(&mainFDB);
				printf("\n");
				printf("VVIP FLIGHTS(Always sorted based on Key) : \n");
				printFDB(&vvipFDB);
				printf("\n");
				printf("VIP FLIGHTS(Always sorted based on Key) : \n");
				printFDB(&vipFDB);
				printf("\n");
				printf("Public FLIGHTS(Always sorted based on Key) : \n");
				printFDB(&publicFDB);
				printf("\n");
				break;
			}

			case 2: //Insert
			{
				printf("Enter how many records you want to enter : ");
				scanf("%d", &numRec);
				printf("Insert\n");
				for (i = 0; i < numRec; i++)
				{
					printf("Enter the following details\n");
					printf("Flight name : ");
					scanf("%s", flight_name);
					printf("Flight id : ");
					scanf("%u", &flight_id);
					printf("Flight capacity : ");
					scanf("%u", &flight_capacity);
					printf("Flight arrival time in the 24-hour format [hh mm] : "); //For Example, input should be "09 21"
					scanf("%hu%hu", &flight_arrival_time.hr, &flight_arrival_time.min);
					printf("Flight departure time in the 24-hour format [hh mm] : "); //For Example, input should be "10 57"
					scanf("%hu%hu", &flight_departure_time.hr, &flight_departure_time.min);
					printf("Flight delay time in the 24-hour format [hh mm] : "); //For Example, input should be "10 57"
					scanf("%hu%hu", &flight_delay_time.hr, &flight_delay_time.min);
					printf("Flight class : ");
					scanf("%s", flight_class);

					sc = insert(&mainFDB, flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class, &vvipFDB, &vipFDB, &publicFDB);
					if (sc == SUCCESS)
						printf("Insert Successful\n");
					else
						printf("Failed to Insert\n");
				}
				break;
			}

			case 3: //Delete
			{
				printf("Enter the flight id of the records you want to delete : ");
				scanf("%u", &flight_id);
				sc = delete (&mainFDB, flight_id, &vvipFDB, &vipFDB, &publicFDB);
				if (sc)
				{
					printf("Successfully deleted");
				}
				else
				{
					printf("Wasn't possible");
				}

				break;
			}

			case 4: //getNumFlights
			{
				printf("The number of flights present are : %d", getNumFlights(&mainFDB));
				break;
			}

			case 5: //Check fullness
			{
				if (isFull(&mainFDB))
				{
					printf("The flight is full\n");
				}
				else
				{
					printf("The flight is NOT full\n");
				}
				break;
			}

			case 6: //Check emptiness
			{
				if (isEmpty(&mainFDB))
				{
					printf("The flight is empty\n");
				}
				else
				{
					printf("The flight is NOT empty\n");
				}
				break;
			}

			case 7: //getSortedOnKey
			{
				getSortedOnKey(&mainFDB);
				printf("The flights have been sorted on basis of Key : \n");
				break;
			}

			case 8: //getSortedOnArrivalTime
			{
				getSortedOnArrivalTime(&mainFDB);
				printf("The flights have been sorted on basis of Arrival Time : \n");
				break;
			}

			case 9: //getSortedOnDepartureTime
			{
				getSortedOnDepartureTime(&mainFDB);
				printf("The flights have been sorted on basis of Departure Time : \n");
				break;
			}

			case 10: //getSortedOnStayTime
			{
				getSortedOnStayTime(&mainFDB);
				printf("The flights have been sorted on basis of Stay Time : \n");
				break;
			}

			case 11: //getFlightWithLongestStay
			{
				getFlightWithtLongestStay(&mainFDB);
				break;
			}

			case 12: //Update Flight Status
			{
				list2 = NULL;
				//Scanf
				printf("Enter the number of records you want to update : ");
				scanf("%d", &numOfRecordsToBeUpdated);
				for (i = 0; i < numOfRecordsToBeUpdated; i++)
				{
					flight_delay_time.hr = 0;
					flight_delay_time.min = 0;

					printf("Enter the following details\n");
					printf("Flight name : ");
					scanf("%s", flight_name);
					printf("Enter the flight id : ");
					scanf("%u", &flight_id);
					printf("Flight arrival time in the 24-hour format [hh mm] : "); //For Example, input should be "09 21"
					scanf("%hu%hu", &flight_arrival_time.hr, &flight_arrival_time.min);
					printf("Enter the updated flight status : (ON_TIME | DELAY | CANCELLED | PROMOTED)");
					scanf("%s", flight_status);
					if (strcmp(flight_status, "DELAY") == 0)
					{
						printf("Flight delay time in the 24-hour format [hh mm] : "); //For Example, input should be "09 21"
						scanf("%hu%hu", &flight_delay_time.hr, &flight_delay_time.min);
					}

					list2 = StatusList_InsertAtStart(list2, flight_name, flight_id, flight_arrival_time, flight_delay_time, flight_status);
				}

				UpdateFlightStatus(&mainFDB, &vvipFDB, &vipFDB, &publicFDB, list2);
				break;
			}

			case 13: //List Unique
			{
				Flight_DataBase SpurFDB;
				initializeDataBase(&SpurFDB);
				SpurFDB.isSortedOnKey = FALSE;

				FR_Node *nptr;
				printf("Enter how many records you want to enter (SPURIOUSLY): ");
				scanf("%d", &numRec);
				printf("Insert\n");
				for (i = 0; i < numRec; i++)
				{
					printf("Enter the following details\n");
					printf("Flight name : ");
					scanf("%s", flight_name);
					printf("Flight id : ");
					scanf("%u", &flight_id);
					printf("Flight capacity : ");
					scanf("%u", &flight_capacity);
					printf("Flight arrival time in the 24-hour format [hh mm] : "); //For Example, input should be "09 21"
					scanf("%hu%hu", &flight_arrival_time.hr, &flight_arrival_time.min);
					printf("Flight departure time in the 24-hour format [hh mm] : "); //For Example, input should be "10 57"
					scanf("%hu%hu", &flight_departure_time.hr, &flight_departure_time.min);
					printf("Flight delay time in the 24-hour format [hh mm] : "); //For Example, input should be "10 57"
					scanf("%hu%hu", &flight_delay_time.hr, &flight_delay_time.min);
					printf("Flight class : ");
					scanf("%s", flight_class);

					//Insert At Start
					nptr = FR_MakeNode(flight_name, flight_id, flight_capacity, flight_arrival_time, flight_departure_time, flight_delay_time, flight_class);
					nptr->next = SpurFDB.listptr;
					SpurFDB.listptr = nptr;
				}

				printf("Database before using listUnique\n");
				printFDB(&SpurFDB);
				listUnique(&SpurFDB);
				printf("Database after using listUnique\n");
				printFDB(&SpurFDB);

				freeFDB(&SpurFDB);
				break;
			}

			default:
			{
				control_line = 0;
				break;
			}
		}
	}

	freeFDB(&mainFDB);
	freeFDB(&vvipFDB);
	freeFDB(&vipFDB);
	freeFDB(&publicFDB);
	free(flight_name);
	free(flight_class);
	free(flight_status);
	return 0;
}