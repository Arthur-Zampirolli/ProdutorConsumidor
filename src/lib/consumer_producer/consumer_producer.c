#include "consumer_producer.h"

#ifndef BUFFER_INCLUDED
#define BUFFER_INCLUDED
  #include "../data.h"
  #include "../data_buffer.h"
#endif



void *ConsumerProducer1(void *arg)
{
    int i, index;
    Data item;
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
        printf("[CP1_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of empty slots */
        sem_post(&shared[0].empty);

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared[1].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[1].mutex);
        shared[1].buf[shared[1].in] = item;
        shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
        printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[1].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[1].full);
    }
    return NULL;
}

void *ConsumerProducer2(void *arg)
{
    int i, index;
    Data item;
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
        printf("[CP1_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of empty slots */
        sem_post(&shared[0].empty);

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared[1].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[1].mutex);
        shared[1].buf[shared[1].in] = item;
        shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
        printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[1].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[1].full);
    }
    return NULL;
}

void *ConsumerProducer3(void *arg)
{
    int i, index;
    Data item;
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
        printf("[CP1_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of empty slots */
        sem_post(&shared[0].empty);

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared[1].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[1].mutex);
        shared[1].buf[shared[1].in] = item;
        shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
        printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[1].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[1].full);
    }
    return NULL;
}
