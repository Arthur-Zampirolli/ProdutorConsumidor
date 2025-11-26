#include "producer.h"
#include "../io.h"
#ifndef BUFFER_INCLUDED
#define BUFFER_INCLUDED
  #include "../data.h"
#endif



//is going to read the file and alocates the matrices in shared memory
Data *loadFromFile(char* filename){
    Data *data = malloc(sizeof(Data));
    data->kill = 0;
    loadMatrices(filename, data->A, data->B);
    initMatrix(data->C);
    return data;
}


void *Producer(void *arg)
{
    int i, index;
    Data item;
    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        /* Produce item */
        //item = i + (index * 1000);


        sem_wait(&shared[0].empty);
        sem_wait(&shared[0].mutex);

        shared[0].buf[shared[0].in] = item;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
        printf("[P_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
        fflush(stdout);
        
        sem_post(&shared[0].mutex);
        sem_post(&shared[0].full);
    }
    printf("sending KILL signal for all CP's...\n");
    for (int j = 0; j < NCP1; j++)
    {
        sem_wait(&shared[0].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[0].mutex);

        printf("Sending kill signal for CP%d\n", j);
        shared[0].buf[shared[0].in].kill = KILL;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[0].full);
    }

    printf("Fim produtor\n");
    return NULL;
}