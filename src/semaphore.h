#ifndef STRUCTS
#define STRUCTS
typedef struct
{
    int buf[BUFF_SIZE]; /* shared var */
    int in;             /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;            /* buf[out%BUFF_SIZE] is the first full slot */
    //propriedades que o professor pediu
    char fileA[NAME_MAX];
    char fileB[NAME_MAX];
    double A[DIMENSION][DIMENSION];
    double B[DIMENSION][DIMENSION];
    double C[DIMENSION][DIMENSION];
    double V[10];
    double E;
    // fim propriedades
    sem_t full;  /* keep track of the number of full spots */
    sem_t empty; /* keep track of the number of empty spots */
    sem_t mutex; /* enforce mutual exclusion to shared data */
} S;

#endif