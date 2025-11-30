#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>
#include <string.h>
#include "lib/constants.h"
// #include "lib/semaphore.h"
#include "lib/io.h"
#include "lib/matrix.h"
#include "lib/threads.h"
#include "lib/data_buffer.h"
#include "lib/constants.h"

S shared[SHARED_COUNT];

void initializeSharedBuffers(){
    for (int index = 0; index < SHARED_COUNT; index++)
    {
        printf("Initializing semaphores for shared[%d]...\n", index);
        sem_init(&shared[index].full, 0, 0);
        sem_init(&shared[index].empty, 0, BUFF_SIZE);
        sem_init(&shared[index].mutex, 0, 1);

        // Inicializa in/out
        shared[index].in = 0;
        shared[index].out = 0;
    }
}


int main(){
    initializeSharedBuffers();
    pthread_t idP[NP], idC[NC], idCP1[NCP1], idCP2[NCP2], idCP3[NCP3];
    int index;
    int sP[NP], sC[NC], sCP1[NCP1], sCP2[NCP2], sCP3[NCP3];

    for (index = 0; index < SHARED_COUNT; index++)
    {
        printf("Initializing semaphores for shared[%d]...\n", index);
        sem_init(&shared[index].full, 0, 0);
        sem_init(&shared[index].empty, 0, BUFF_SIZE);
        sem_init(&shared[index].mutex, 0, 1);

        // Inicializa in/out
        shared[index].in = 0;
        shared[index].out = 0;
    }

    for (index = 0; index < NP; index++)
    {
        printf("Creating producer thread %d...\n", index);
        sP[index] = index;
        /* Create a new producer */
        pthread_create(&idP[index], NULL, Producer, &sP[index]);
    }



    //CONSUMER PRODUCER THREADS
    for (index = 0; index < NCP1; index++)
    {
        printf("Creating consumer-producer1 thread %d...\n", index);
        sCP1[index] = index;
        /* Create a new producer */
        pthread_create(&idCP1[index], NULL, ConsumerProducer1, &sCP1[index]);
    }

    for (index = 0; index < NCP2; index++)
    {
        printf("Creating consumer-producer2 thread %d...\n", index);
        sCP2[index] = index;
        /* Create a new producer */
        pthread_create(&idCP2[index], NULL, ConsumerProducer2, &sCP2[index]);
    }

    for (index = 0; index < NCP3; index++)
    {
        printf("Creating consumer-producer3 thread %d...\n", index);
        sCP3[index] = index;
        /* Create a new producer */
        pthread_create(&idCP3[index], NULL, ConsumerProducer3, &sCP3[index]);
    }



    // //CONSUMER THREADS
    for (index = 0; index < NC; index++)
    {
        printf("Creating consumer thread %d...\n", index);
        sC[index] = index;
        /* Create a new consumer */
        pthread_create(&idC[index], NULL, Consumer, &sC[index]);
    }
    //JOIN THREADS
    for (index = 0; index < NP; index++)
    {
        pthread_join(idP[index], NULL);
    }


    for (index = 0; index < NCP1; index++)
    {
        pthread_join(idCP1[index], NULL);
    }
    for (index = 0; index < NCP2; index++)
    {
        pthread_join(idCP2[index], NULL);
    }
    for (index = 0; index < NCP3; index++)
    {
        pthread_join(idCP3[index], NULL);
    }



    for (index = 0; index < NC; index++)
    {
        pthread_join(idC[index], NULL);
    }

    for (index = 0; index < SHARED_COUNT; index++)
    {
        sem_destroy(&shared[index].full);
        sem_destroy(&shared[index].empty);
        sem_destroy(&shared[index].mutex);
    }

    printf("Programa encerrado com sucesso.\n");
    return 0;
}
