#ifndef DATA_H
#define DATA_H
#include "constants.h"
#include </usr/include/semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
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
} S;

extern S shared[5];

Data *initializeData();
void showData(Data *data);

#endif