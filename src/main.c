#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>
#include <string.h>
#include "constants.h"
#include "semaphore.h"
#include "io.h"

S shared[BUFF_SIZE]; //5 posiçoes como pedido

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

// int main()
// {
//     pthread_t idP, idC, idCP;
//     int index;
//     int sP[NP], sC[NC], sCP[NCP];

//     for (index = 0; index < 2; index++)
//     {
//         sem_init(&shared[index].full, 0, 0);
//         sem_init(&shared[index].empty, 0, BUFF_SIZE);
//         sem_init(&shared[index].mutex, 0, 1);
//     }

//     for (index = 0; index < NP; index++)
//     {
//         sP[index] = index;
//         /* Create a new producer */
//         pthread_create(&idP, NULL, Producer, &sP[index]);
//     }

//     for (index = 0; index < NCP; index++)
//     {
//         sCP[index] = index;
//         /* Create a new producer */
//         pthread_create(&idCP, NULL, ConsumerProducer, &sCP[index]);
//     }

//     for (index = 0; index < NC; index++)
//     {
//         sC[index] = index;
//         /* Create a new consumer */
//         pthread_create(&idC, NULL, Consumer, &sC[index]);
//     }

//     pthread_exit(NULL);
// }

int main(){
    //int size = 50;
    double A[DIMENSION][DIMENSION];
    double B[DIMENSION][DIMENSION];
    char fileInput[STRING_MAX+6];
    
    FILE *fp = fopen("./input/entrada.in", "r");
    if(!fp){
        perror("File error:\n");
        return 1;
    }
    char buffer[STRING_MAX];
    while(fgets(buffer, STRING_MAX, fp) != NULL){
        int size = strlen(buffer);
        buffer[size - 1] = '\0';
        if(size <= 1){
            //EOF
            continue;
        }
        if(size > 1){
            
            sprintf(fileInput, "%s", buffer);
            
            //fileInput[size - 1] = '\0'
            
            printf("Loading file: %s\n", fileInput);
            loadMatrices(fileInput, A, B);
        }

    }
    fclose(fp);
    return 0;
}


