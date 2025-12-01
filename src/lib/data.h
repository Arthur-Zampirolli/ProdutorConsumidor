#ifndef DATA_H
#define DATA_H
#include "constants.h"
#include </usr/include/semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
typedef struct data
{
    //propriedades que o professor pediu
    char fileA[NAME_MAX];
    char fileB[NAME_MAX];
    double A[DIMENSION][DIMENSION];
    double B[DIMENSION][DIMENSION];
    double C[DIMENSION][DIMENSION];
    double V[10];
    double E;
    int kill;
    int consumed;
} Data;

typedef struct
{
    Data buf[BUFF_SIZE]; /* shared var */
    int in;             /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;            /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;         /* keep track of the number of full spots */
    sem_t empty;        /* keep track of the number of empty spots */
    sem_t mutex;        /* enforce mutual exclusion to shared data */
    int kill;
    int consumed;
}S;

extern S shared[SHARED_COUNT];
extern int fileLines;
extern int kill_propagated_NCP2;
extern pthread_mutex_t kill_mutex_NCP2;
extern int kill_propagated_NCP3;
extern pthread_mutex_t kill_mutex_NCP3;
extern int kill_propagated_NC;
extern pthread_mutex_t kill_mutex_NC;

Data *initializeData();
void showData(Data *data);

#endif