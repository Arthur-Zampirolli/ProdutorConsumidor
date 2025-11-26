#include "consumer_producer.h"
#include "matrix.h"
#ifndef BUFFER_INCLUDED
#define BUFFER_INCLUDED
  #include "../data.h"
  #include "../data_buffer.h"
#endif


//CP1 - Thread Consumidora & Produtora 1 - Move shared[0]→buer[out] para um ponteiro temporário,
//calcula C=A*B no elemento temporário e move o ponteiro temporário para shared[1]→buer[in]. 
// Teremos 5 instâncias desta thread.

void *createCPPointer(Data *item){
    Data *ptr = (Data *)malloc(sizeof(Data));

    if (ptr == NULL)
    {
        fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    copyMatrix(item->A, ptr->A);
    copyMatrix(item->B, ptr->B);
    copyMatrix(item->C, ptr->C);
    ptr->kill = item->kill;
    strncpy(ptr->fileA, item->fileA, NAME_MAX);
    strncpy(ptr->fileB, item->fileB, NAME_MAX);
    memcpy(ptr->V, item->V, sizeof(double)*10);
    return (void *)ptr;
}





void *ConsumerProducer1(void *arg)
{
    int i, index;
    Data item;
    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        /*BEGIN CRITICAL REGION*/
        sem_wait(&shared[0].full);
        sem_wait(&shared[0].mutex);
        
            item = shared[0].buf[shared[0].out];
            Data *ptr = (Data *)createCPPointer(&item);
            matrixMultiply(ptr->A, ptr->B, ptr->C);
            shared[0].out = (shared[0].out + 1) % BUFF_SIZE;
            printf("[CP1_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[0].mutex);
        sem_post(&shared[0].empty);
        /*END CRITICAL REGION*/

        /*BEGIN CRITICAL REGION*/
        sem_wait(&shared[1].empty);
        sem_wait(&shared[1].mutex);

            shared[1].buf[shared[1].in] = *ptr;
            shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
            printf("[CP1_%d] Producing %s %s...\n", index, ptr->fileA, ptr->fileB);
            fflush(stdout);

        sem_post(&shared[1].mutex);
        sem_post(&shared[1].full);
        /*END CRITICAL REGION*/

        free(ptr);
    }
    return NULL;
}


//CP2 - Thread Consumidora & Produtora 2 Move shared[1]→buffer[out] para um ponteiro temporário,
//calcula V como a soma das colunas de C. (teremos 4 instâncias desta thread)
void *ConsumerProducer2(void *arg)
{
    int i, index;
    Data item;
    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        // BEGIN CRITICAL REGION
        sem_wait(&shared[1].full);
        sem_wait(&shared[1].mutex);

            item = shared[1].buf[shared[1].out];
            Data *ptr = (Data *)createCPPointer(&item);
            shared[1].out = (shared[1].out + 1) % BUFF_SIZE;
            printf("[CP1_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);
            sumColumns(ptr->C, ptr->V);

        sem_post(&shared[1].mutex);
        sem_post(&shared[1].empty);
        // END CRITICAL REGION

        // BEGIN CRITICAL REGION
        sem_wait(&shared[2].empty);
        sem_wait(&shared[2].mutex);

            shared[2].buf[shared[2].in] = item;
            shared[2].in = (shared[2].in + 1) % BUFF_SIZE;
            printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[2].mutex);
        sem_post(&shared[2].full);
        // END CRITICAL REGION
    }
    return NULL;
}


//CP3 - Thread Consumidora & Produtora 3 Move shared[2]→buffer[out] para um ponteiro temporário,
//calcula E como a soma dos elementos de V. Move o ponteiro temporário para shared[3]→buffer[in). Teremos 2 instâncias desta thread.
void *ConsumerProducer3(void *arg)
{
    int i, index;
    Data item;
    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {
        //BEGIN CRITICAL REGION
        sem_wait(&shared[2].full);
        sem_wait(&shared[2].mutex);

            item = shared[2].buf[shared[2].out];
            Data *ptr = (Data *)createCPPointer(&item);
            sumV(ptr->V, ptr->E);
            shared[2].out = (shared[2].out + 1) % BUFF_SIZE;
            printf("[CP1_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[2].mutex);
        sem_post(&shared[2].empty);
        //END CRITICAL REGION

        //BEGIN CRITICAL REGION
        sem_wait(&shared[3].empty);
        sem_wait(&shared[3].mutex);

            shared[3].buf[shared[3].in] = item;
            shared[3].in = (shared[3].in + 1) % BUFF_SIZE;
            printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[1].mutex);
        sem_post(&shared[1].full);
        //END CRITICAL REGION
    }
    return NULL;
}
