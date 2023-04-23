/**************************************************
 *                                                *
 * file: twitter.h                                *
 *                                                *
 *                                                *
 * @Author  Iacovos G. Kolokasis                  *
 * @Version 25-02-2020                            *
 * @email   kolokasis@ics.forth.gr                *
 *                                                *
 * @brief   Header file for the needs of cs-240b  *
 * project 2018, with the structures and function *
 * prototypes                                     * 
 *                                                *
 **************************************************
 */

#ifndef __AIRLINES_H_
#define __AIRLINES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DEST 10                             /* Maximum number of destinations    */

/**
 * Structure defining a node of the airlines double linked list
 * tree
 */
typedef struct airlines {
    int aId;                                    /* Airline  identifier                  */
    struct airplanes *p_root;                   /* Pointer to the airplane list         */
    struct airlines *next;                      /* Pointer to the next airline node     */
    struct airlines *prev;                      /* Pointer to the previous airline node */
} _airlines_l;

/**
 * Structure defining a node of the airplane linked list
 */
typedef struct airplanes {
    int pId;                                    /* Airplane identifier               */   
    int dest;                                   /* Destination Id                    */
    int depart_time;                            /* Departute time                    */
    struct airplanes *next;                     /* Pointer to the next node          */
} _airplane;

/**
 * Structure defining the flights list
 */
typedef struct flights {
    int pId;                                    /* Airplane identifier                */
    int depart_time;                            /* Departure time                     */
    struct flights *next;                       /* Pointer to the next node           */
} _flights_l;

/**
 * Structure defining the destination table
 */
typedef struct destinations {
    struct flights *flight_p;                /* Pointer to the flight list         */
    struct flights *sentinel_p;              /* Pointer to the flight list         */
} _destinations_p;

/* Global variable, pointer to the head of the airlines list */
extern _airlines_l *airlines_p;
 
/* Global variable, destination table */
extern _destinations_p destTable[MAX_DEST];

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
 * @param aId The user's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_airline(int aId);

/**
 * @brief Insert new airplane to the list of the certain airline with aId
 *
 * @param aId         The airlines id
 * @param pId         The airplanes id
 * @param dest        Destination Id [0-9]
 * @param depart_time Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_airplanes(int aId, int pId, int dest, int depart_time);

/**
 * @brief Cancel flight
 *
 * @param aId         The airlines id
 * @param pId         The airplanes id
 * @param dest        Destination Id [0-9]
 *
 * @return 1 on success
 *         0 on failure
 */
int cancel_flight(int aId, int pId, int dest);

/**
 * @brief Delete airline
 *
 * @param aId         The airlines id
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_airline(int aId);

/**
 * @brief Acquisition airline
 *
 * @param aId1         The first airline id
 * @param aId2         The second airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int acquisition_airline(int aId1, int aId2);

/**
 * @brief Subsidiary company
 *
 * @param aId1         The first airline id
 * @param aId2         The second airline id
 * @param dest         Destination
 *
 * @return 1 on success
 *         0 on failure
 */
int subsidiary_airiline(int aId1, int aId2, int dest);

/**
 * @brief Partition airplanes
 *
 * @param aId         The airline id
 *
 * @return 1 on success
 *         0 on failure
 */
int partition_airplanes(int aId1);

/**
 * @brief Travel
 *
 * @param dest         Destination
 * @param timestamp    Departure time
 *
 * @return 1 on success
 *         0 on failure
 */
int travel(int dest, int timestamp);

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
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void);

#endif /* __AIRLINES_H_ */
