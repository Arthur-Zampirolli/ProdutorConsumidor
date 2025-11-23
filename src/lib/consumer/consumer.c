#include "consumer.h"

#ifndef BUFFER_INCLUDED
#define BUFFER_INCLUDED
  #include "../data.h"
  #include "../data_buffer.h"
#endif



void *Consumer(void *arg)
{
    int i, index;
    Data item;
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
        printf("[C_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        /* Release the buffer */
        sem_post(&shared[1].mutex);
        /* Increment the number of empty slots */
        sem_post(&shared[1].empty);
    }
    return NULL;
}