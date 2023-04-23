/**************************************************
 *                                                *
 * file:        airlines.h                        *
 * Header file for cs-240b project 2020 (phase B) *
 * with the structures and function prototypes    *
 *                                                *
 * @Author:     Karampelas Apostolos              *
 * @Version:    23-04-2020                        *
 * @email:      apskar@csd.uoc.gr                 *
 *                                                *
 **************************************************/

#ifndef __AIRLINES_H_
#define __AIRLINES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DEST 10                            /* Maximum number of destinations    */

/**
 * Structure defining a node of the airlines double linked list
 * tree
 */
typedef struct airline {
    int aid;                                   /* Airline  identifier                  */
    struct airplane *pR;                       /* Pointer to the airplane list         */
    struct airline *next;                      /* Pointer to the next airline node     */
    struct airline *prev;                      /* Pointer to the previous airline node */
} _airline;

/**
 * Structure defining a node of the airplane binary search tree
 */
typedef struct airplane {
    int pid;                                    /* Airplane identifier               */
    int dest;                                   /* Destination Id                    */
    int depart_time;                            /* Departute time                    */
    struct airplane *lc;                        /* Pointer to the node's left child  */
    struct airplane *rc;                        /* Pointer to the node's right child */
} _airplane;

/**
* Structure defining a node of the flight threaded binary search tree
*/
typedef struct flight {
    int pid;                                    /* Airplane identifier                                    */
    int depart_time;                            /* Departure time                                         */
    int thread_status;                          /* Indicator of threads in the node's lc or rc            */
    struct flight *lc;                          /* Pointer to the node's left child or inline predecessor */
    struct flight *rc;                          /* Pointer to the node's right child or inline successor  */
} _flight;

/**
* Structure defining a node of the client priority queue
*/
typedef struct client {
    int cid;                                    /* Client identifier                  */
    int miles;                                  /* Flying miles                       */
    struct client *lc;                          /* Pointer to the node's left child   */
    struct client *mc;                          /* Pointer to the node's middle child */
    struct client *rc;                          /* Pointer to the node's right child  */
    struct client *p;                           /* Pointer to the node's parent       */
} _client;

/**
* Structure defining the client priority queue
*/
typedef struct priority_queue {
    struct client *Q;                           /* Pointer to the root of the tree of clients */
    int num_elements;                           /* Number of clients in the queue */
} _priority_queue;

typedef struct fifo_queue {
    struct client **Q;                      /* dynamically allocated array to store queue elements */
                                        /* used for level-order traversals of priority queue */
    int front;
    int length;
} _fifo_queue;

/* Global variable, pointer to the head of the airlines list */
extern _airline *airlines;

/* Global variable, destination table */
extern _flight *DESTINATIONS[MAX_DEST];

/* Global variable, pointer to the priority queue */
extern _priority_queue *PQ;

/* Global variable, pointer to the fifo queue */
extern _fifo_queue *FQ;

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize();

/**
 * @brief Register airline
 *
 * @param aid The user's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_airline(int aid);

/**
 * @brief Insert new airplane to the list
 *
 * @param pid         The airplane's id
 * @param aid         The airline's id
 * @param dest        Destination Id [0-9]
 * @param depart_time Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_airplane(int pid, int aid, int dest, int depart_time);

/**
 * @brief Cancel flight
 *
 * @param aid         The airline's id
 * @param pid         The airplane's id
 * @param dest        Destination Id [0-9]
 *
 * @return 1 on success
 *         0 on failure
 */
int cancel_flight(int aid, int pid, int dest);

/**
 * @brief Delete airline
 *
 * @param aid         The airline's id
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_airline(int aid);

/**
 * @brief Acquisition airline
 *
 * @param aid1         The first airline's id
 * @param aid2         The second airline's id
 *
 * @return 1 on success
 *         0 on failure
 */
int acquisition_airline(int aid1, int aid2);

/**
 * @brief Subsidiary company
 *
 * @param aid1         The first airline's id
 * @param aid2         The second airline's id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int subsidiary_airiline(int aid1, int aid2, int dest);

/**
 * @brief Find flight
 *
 * @param dest         Destination
 * @param ts           Timestamp
 *
 * @return 1 on success
 *         0 on failure
 */
int find_flight(int dest, int ts);

/**
 * @brief New client
 *
 * @param cid        The client's id
 *
 * @return 1 on success
 *         0 on failure
 */
int new_client(int cid);

/**
 * @brief Erase client with the most miles
 *
 *
 * @return 1 on success
 *         0 on failure
 */
int erase_client();


/**
 * @brief Travel client
 *
 * @param cid          The client's id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int travel_client(int cid, int dest);

/**
 * @brief Print airlines
 *
 * @return 1 on success
 *         0 on failure
 */
int print_airlines();

/**
 * @brief Print destinations
 *
 * @return 1 on success
 *         0 on failure
 */
int print_destinations();

/**
 * @brief Print clients
 *
 * @return 1 on success
 *         0 on failure
 */
int print_clients();

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void);

#endif /* __AIRLINES_H_ */
