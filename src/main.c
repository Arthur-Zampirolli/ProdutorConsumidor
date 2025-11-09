#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>

#define BUFF_SIZE 5 /* total number of slots */
#define NP 3        /* total number of producers */
#define NCP 3       /* total number of consumers/producers */
#define NC 1        /* total number of consumers */
#define NITERS 4    /* number of items produced/consumed */

#define DIMENSION 10 //dimensao da matrix
#define NAME_MAX 100 // tamanho maximo do nome do arquivo
//path de arquivos de entrada
char inputPath[] = "./input/";
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
} sbuf_t;

sbuf_t shared[2];

void *Producer(void *arg)
{
    int i, item, index;

    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        /* Produce item */
        item = i + (index * 1000);

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared[0].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[0].mutex);
        shared[0].buf[shared[0].in] = item;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
        printf("[P_%d] Producing %d ...\n", index, item);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[0].full);
    }
    return NULL;
}

void *ConsumerProducer(void *arg)
{
    int i, item, index;

    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        /* Prepare to read item from buf */

        /* If there are no filled slots, wait */
        sem_wait(&shared[0].full);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[0].mutex);
        item = shared[0].buf[shared[0].out];
        shared[0].out = (shared[0].out + 1) % BUFF_SIZE;
        printf("[CP1_%d] Consuming %d ...\n", index, item);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of empty slots */
        sem_post(&shared[0].empty);

        // Change consumed item
        item += 500;

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared[1].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[1].mutex);
        shared[1].buf[shared[1].in] = item;
        shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
        printf("[CP1_%d] Producing %d ...\n", index, item);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[1].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[1].full);
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, item, index;

    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        /* Prepare to read item from buf */

        /* If there are no filled slots, wait */
        sem_wait(&shared[1].full);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[1].mutex);
        item = shared[1].buf[shared[1].out];
        shared[1].out = (shared[1].out + 1) % BUFF_SIZE;
        printf("[C_%d] Consuming %d ...\n", index, item);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[1].mutex);
        /* Increment the number of empty slots */
        sem_post(&shared[1].empty);
    }
    return NULL;
}

int main()
{
    pthread_t idP, idC, idCP;
    int index;
    int sP[NP], sC[NC], sCP[NCP];

    for (index = 0; index < 2; index++)
    {
        sem_init(&shared[index].full, 0, 0);
        sem_init(&shared[index].empty, 0, BUFF_SIZE);
        sem_init(&shared[index].mutex, 0, 1);
    }

    for (index = 0; index < NP; index++)
    {
        sP[index] = index;
        /* Create a new producer */
        pthread_create(&idP, NULL, Producer, &sP[index]);
    }

    for (index = 0; index < NCP; index++)
    {
        sCP[index] = index;
        /* Create a new producer */
        pthread_create(&idCP, NULL, ConsumerProducer, &sCP[index]);
    }

    for (index = 0; index < NC; index++)
    {
        sC[index] = index;
        /* Create a new consumer */
        pthread_create(&idC, NULL, Consumer, &sC[index]);
    }

    pthread_exit(NULL);
}

void *matrixMultiply(double **a, double **b, double **c)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 10; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
                /*Exemplo: c[0][0] = a[0][0]*b[0][0]+
                a[0][1]*b[1][0]+
                a[0][2]*b[2][0];

                */
            }
        }
    }
}