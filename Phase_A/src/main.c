/***************************************************************
 *                                                             *
 * file: main.c                                                *
 *                                                             *
 * @Author  Iacovos G. Kolokasis                               *
 * @Version 25-02-2020                                         *
 * @email   kolokasis@csd.forth.gr                             *
 *                                                             *
 * @brief   Main function for the needs of cs-240b project 2018*
 *                                                             *
 ***************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "airlines.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

/* Global variable, pointer to the head of the airlines list */
_airlines_l *airlines_p;
 
/* Global variable, destination table */
_destinations_p destTable[MAX_DEST];


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
    FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> <>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	initialize();
	
    /* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {
        
		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

			/* Register airline
			 * R <aId> */
			case 'R':
				{
					int aId;

					sscanf(buff, "%c %d", &event, &aId);
					DPRINT("%c %d\n", event, aId);

					if (register_airline(aId)) {
						DPRINT("R %d succeeded\n", aId);
					} else {
						fprintf(stderr, "R %d failed\n", aId);
					}

					break;
				}

            /* Insert new airplane
			 * I <aId> <pId> <dest> <depart_time> */
			case 'I':
				{
					int aId, pId, dest, depart_time;

					sscanf(buff, "%c %d %d %d %d", &event, &aId, &pId, &dest, &depart_time);
					DPRINT("%c %d %d %d %d\n", event, aId, pId, dest, depart_time);

					if (insert_airplanes(aId, pId, dest, depart_time)) {
						DPRINT("%c %d %d %d %d succeeded\n", event, aId, pId, dest, depart_time);
					} else {
						fprintf(stderr, "%c %d %d %d %d failed\n", event, aId, pId, dest, depart_time);
					}

					break;
				}

			/* Cancel flight
			 * C <aId> <pId> <dest> */
			case 'C':
				{
					int aId, pId, dest;

					sscanf(buff, "%c %d %d %d", &event, &aId, &pId, &dest);
					DPRINT("%c %d %d %d", event, aId, pId, dest);

					if (cancel_flight(aId, pId, dest)) {
						DPRINT("%c %d %d %d succeeded\n", event, aId, pId, dest);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, aId, pId, dest);
					}

					break;
				}

			/* Delete airline
			 * D <aId> */
			case 'D':
				{
					int aId;

					sscanf(buff, "%c %d", &event, &aId);
					DPRINT("%c %d \n", event, aId);

					if (delete_airline(aId)) {
						DPRINT("%c %d succeeded\n", event, aId);
					} else {
						fprintf(stderr, "%c %d failed\n", event, aId);
					}

					break;
				}

			/* Acquisition airline
			 * A <uid1> <uid2>*/
			case 'A':
				{
					int aId1, aId2;

					sscanf(buff, "%c %d %d", &event, &aId1, &aId2);
					DPRINT("%c %d %d\n", event, aId1, aId2);

					if (acquisition_airline(aId1, aId2)) {
						DPRINT("%c %d %d succeeded\n", event, aId1, aId2);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, aId2, aId2);
					}

					break;
				}

                /* Subsidiary airline
                 * S <aId1> <aId2> <dest>
                 */
            case 'S':
                {
                    int aId1, aId2, dest;

                    sscanf(buff, "%c %d %d %d", &event, &aId1, &aId2, &dest);
					DPRINT("%c %d %d %d\n", event, aId1, aId2, dest);

                    if (subsidiary_airiline(aId1, aId2, dest)) {
                        DPRINT("%c %d %d %d succeded\n", event, aId1, aId2, dest);
                    } else {
                        fprintf(stderr, "%c %d %d %d failed\n", event, aId1, aId2, dest);
                    }

                    break;
                }

			/* Partition airplanes
			 * P <aId> */
			case 'P':
				{
					int aId;

					sscanf(buff, "%c %d\n", &event, &aId);
					DPRINT("%c %d \n", event, aId);

					if (partition_airplanes(aId)) {
						DPRINT("%c %d succeeded\n", event, aId);
					} else {
						fprintf(stderr, "%c %d failed\n", event, aId);
					}

					break;
				}
			
            /* Travel 
			 * T <dest> <timestamp> */
			case 'T':
				{
					int dest, timestamp;

					sscanf(buff, "%c %d %d\n", &event, &dest, &timestamp);
					DPRINT("%c %d %d\n", event, dest, timestamp);

					if (travel(dest, timestamp)) {
						DPRINT("%c %d %d succeeded\n", event, dest, timestamp);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, dest, timestamp);
					}

					break;
				}

			/* Print airlines
			 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_airlines()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print destinations flights
			 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_destinations()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Empty line */
			case '\n':
				break;

			/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
