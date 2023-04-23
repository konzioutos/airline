/***************************************************
 *                                                 *
 * file:        main.c                             *
 * Main file for cs-240b project 2020 (phase B)    *
 *                                                 *
 * @Author:     Apostolos Karampelas               *
 * @Version:    29-04-2020                         *
 * @email:      apskar@csd.uoc.gr                  *
 *                                                 *
 ***************************************************/

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
_airline *airlines;

/* Global variable, destination table */
_flight *DESTINATIONS[MAX_DEST];

/* Global variable, pointer to the priority queue */
_priority_queue *PQ;

/* Global variable, pointer to the fifo queue */
_fifo_queue *FQ;

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
			 * R <aid> */
			case 'R':
			{
				int aid;

				sscanf(buff, "%c %d", &event, &aid);
				DPRINT("%c %d\n", event, aid);

				if (register_airline(aid)) {
					DPRINT("R %d succeeded\n", aid);
				} else {
					fprintf(stderr, "R %d failed\n", aid);
				}

				break;
			}

            /* Insert new airplane
			 * I <pid> <aid> <dest> <depart_time> */
			case 'I':
			{
				int pid, aid, dest, depart_time;

				sscanf(buff, "%c %d %d %d %d", &event, &pid, &aid, &dest, &depart_time);
				DPRINT("%c %d %d %d %d\n", event, pid, aid, dest, depart_time);

				if (insert_airplane(pid, aid, dest, depart_time)) {
					DPRINT("%c %d %d %d %d succeeded\n", event, pid, aid, dest, depart_time);
				} else {
					fprintf(stderr, "%c %d %d %d %d failed\n", event, pid, aid, dest, depart_time);
				}

				break;
			}

			/* Cancel flight
			 * C <aid> <pid> <dest> */
			case 'C':
			{
				int aid, pid, dest;

				sscanf(buff, "%c %d %d %d", &event, &aid, &pid, &dest);
				DPRINT("%c %d %d %d", event, aid, pid, dest);

				if (cancel_flight(aid, pid, dest)) {
					DPRINT("%c %d %d %d succeeded\n", event, aid, pid, dest);
				} else {
					fprintf(stderr, "%c %d %d %d failed\n", event, aid, pid, dest);
				}

				break;
			}

			/* Delete airline
			 * D <aid> */
			case 'D':
			{
				int aid;

				sscanf(buff, "%c %d", &event, &aid);
				DPRINT("%c %d \n", event, aid);

				if (delete_airline(aid)) {
					DPRINT("%c %d succeeded\n", event, aid);
				} else {
					fprintf(stderr, "%c %d failed\n", event, aid);
				}

				break;
			}

			/* Acquisition airline
			 * A <uid1> <uid2>*/
			case 'A':
			{
				int aid1, aid2;

				sscanf(buff, "%c %d %d", &event, &aid1, &aid2);
				DPRINT("%c %d %d\n", event, aid1, aid2);

				if (acquisition_airline(aid1, aid2)) {
					DPRINT("%c %d %d succeeded\n", event, aid1, aid2);
				} else {
					fprintf(stderr, "%c %d %d failed\n", event, aid2, aid2);
				}

				break;
			}

            /* Subsidiary airline
             * S <aid1> <aid2> <dest>
             */
            case 'S':
            {
                int aid1, aid2, dest;

                sscanf(buff, "%c %d %d %d", &event, &aid1, &aid2, &dest);
				DPRINT("%c %d %d %d\n", event, aid1, aid2, dest);

                if (subsidiary_airiline(aid1, aid2, dest)) {
                    DPRINT("%c %d %d %d succeded\n", event, aid1, aid2, dest);
                } else {
                    fprintf(stderr, "%c %d %d %d failed\n", event, aid1, aid2, dest);
                }

                break;
            }

			/* Find flight
			 * F <dest> <ts> */
			case 'F':
			{
				int dest, ts;

				sscanf(buff, "%c %d %d\n", &event, &dest, &ts);
				DPRINT("%c %d %d\n", event, dest, ts);

				if (find_flight(dest, ts)) {
					DPRINT("%c %d %d succeeded\n", event, dest, ts);
				} else {
					fprintf(stderr, "%c %d %d failed\n", event, dest, ts);
				}

				break;
			}

            /* New client
			 * N <cid> */
			case 'N':
			{
				int cid;

				sscanf(buff, "%c %d\n", &event, &cid);
				DPRINT("%c %d\n", event, cid);

				if (new_client(cid)) {
					DPRINT("%c %d succeeded\n", event, cid);
				} else {
					fprintf(stderr, "%c %d failed\n", event, cid);
				}

				break;
			}

            /* Erase client
			 * E */
			case 'E':
			{
				sscanf(buff, "%c\n", &event);
				DPRINT("%c\n", event);

				if (erase_client()) {
					DPRINT("%c succeeded\n", event);
				} else {
					fprintf(stderr, "%c failed\n", event);
				}

				break;
			}

            /* Travel Client
			 * T <cid> <dest> */
			case 'T':
			{
				int cid, dest;

				sscanf(buff, "%c %d %d\n", &event, &cid, &dest);
				DPRINT("%c %d %d\n", event, cid, dest);

				if (travel_client(cid, dest)) {
					DPRINT("%c %d %d succeeded\n", event, cid, dest);
				} else {
					fprintf(stderr, "%c %d %d failed\n", event, cid, dest);
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

            /* Print clients
             * Z */
            case 'Z':
            {
                sscanf(buff, "%c", &event);
                DPRINT("%c\n", event);

                if (print_clients()) {
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
