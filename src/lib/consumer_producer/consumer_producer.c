#include "consumer_producer.h"
#include "../matrix.h"
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
    //printf("Allocating memory for CP Pointer...\n");
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
    memcpy(ptr->V, item->V, sizeof(double*)*DIMENSION);

    ptr->E = item->E;
    //ptr->consumed = item->consumed;
    return (void *)ptr;
}

void kill_threads_NCP2() {
  for (int i = 0; i < NCP2; i++) {
    Data *kill_item = (Data *) malloc(sizeof(Data));
    kill_item->kill = KILL;

    // BEGIN CRITICAL REGION
    sem_wait(&shared[1].empty);
    sem_wait(&shared[1].mutex);

        shared[1].buf[shared[1].in] = *kill_item;
        shared[1].in = (shared[1].in + 1) % BUFF_SIZE;

    sem_post(&shared[1].full);
    sem_post(&shared[1].mutex);
    // END CRITICAL REGION
    free(kill_item);
  }
}

void kill_threads_NCP3() {
  for (int i = 0; i < NCP3; i++) {
    Data *kill_item = (Data *) malloc(sizeof(Data));
    kill_item->kill = KILL;

    //BEGIN CRITICAL REGION
    sem_wait(&shared[2].empty);
    sem_wait(&shared[2].mutex);

        shared[2].buf[shared[2].in] = *kill_item;
        shared[2].in = (shared[2].in + 1) % BUFF_SIZE;

    
    sem_post(&shared[2].full);
    sem_post(&shared[2].mutex);
    //END CRITICAL REGION
    free(kill_item);
  }
}


void kill_threads_NC() {
  for (int i = 0; i < NC; i++) {
    Data *kill_item = (Data *) malloc(sizeof(Data));
    kill_item->kill = KILL;

    //BEGIN CRITICAL REGION
    sem_wait(&shared[3].empty);
    sem_wait(&shared[3].mutex);

        shared[3].buf[shared[3].in] = *kill_item;
        shared[3].in = (shared[3].in + 1) % BUFF_SIZE;

    sem_post(&shared[3].full);
    sem_post(&shared[3].mutex);
    //END CRITICAL REGION
    free(kill_item);
  }
}

void showItem(Data *item){
    printf("Item contents:\n");
    printf("fileA: %s\n", item->fileA);
    printf("fileB: %s\n", item->fileB);
    printf("Matrix A:\n");
    printMatrix(item->A);
    printf("Matrix B:\n");
    printMatrix(item->B);
    printf("Matrix C:\n");
    printMatrix(item->C);
    printf("Vector V:\n");
    for(int i = 0; i < DIMENSION; i++){
        printf("%lf ", item->V[i]);
    }
    printf("\nE: %lf\n", item->E);
}

// void *ConsumerProducer1(void *arg)
// {
//     int index;
//     Data item;

//     index = *((int *)arg);
//     printf("Starting CP1 thread %d...\n", index);
//     while(1)
//     {
//         printf("CP1_%d waiting to consume...\n", index);
//         //int consumed = 0;
//         /*BEGIN CRITICAL REGION*/
//         sem_wait(&shared[0].full);
//         sem_wait(&shared[0].mutex);

//             item = shared[0].buf[shared[0].out];
//             shared[0].out = (shared[0].out + 1) % BUFF_SIZE;
//             fflush(stdout);

//         sem_post(&shared[0].mutex);
//         sem_post(&shared[0].empty);
//         /*END CRITICAL REGION*/
//         matrixMultiply(item.A, item.B, item.C);

//         if (item.kill == KILL) {
//            kill_threads_NCP2();
//            printf("[CP1_%d] Received kill message\n", index); fflush(stdout);
//            break;
//         }
//         // if(consumed){
//         //     printf("[CP1_%d] Item %s already consumed, skipping...\n", index, item.fileA); fflush(stdout);
//         //     free(ptr);
//         //     continue;
//         // }
//         /*BEGIN CRITICAL REGION*/
//         sem_wait(&shared[1].empty);
//         sem_wait(&shared[1].mutex);

//             shared[1].buf[shared[1].in] = item;
//             shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
//             printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
//             fflush(stdout);

//         sem_post(&shared[1].mutex);
//         sem_post(&shared[1].full);
//         /*END CRITICAL REGION*/

//         //free(ptr);
//     }
//     return NULL;
// }

void *ConsumerProducer1(void *arg)
{
    int index;
    Data item;

    index = *((int *)arg);
    printf("Starting CP1 thread %d...\n", index);
    
    while(1)
    {
        printf("[CP1_%d] Waiting to consume...\n", index);
        printSharedBuffer(&shared[1], "[CP1]");
        /* BEGIN CRITICAL REGION (Consumo do Buffer 0) */
        sem_wait(&shared[0].full);  // Espera por item
        sem_wait(&shared[0].mutex); // Espera por acesso exclusivo

            item = shared[0].buf[shared[0].out];
            shared[0].out = (shared[0].out + 1) % BUFF_SIZE;
            fflush(stdout);

        sem_post(&shared[0].mutex);
        sem_post(&shared[0].empty); 
        /* END CRITICAL REGION */

        if (item.kill == KILL) {
           
           pthread_mutex_lock(&kill_mutex_NCP2);
           
           if (kill_propagated_NCP2 == 0) {
               kill_threads_NCP2(); 
               kill_propagated_NCP2 = 1;
               printf("[CP1_LIDER] Propagando KILL para CP2.\n");
           }
           
           pthread_mutex_unlock(&kill_mutex_NCP2);

           printf("[CP1_%d] Consumed KILL and terminating.\n", index); 
           fflush(stdout);
           return NULL; // Termina a thread CP1
        }
        
        matrixMultiply(item.A, item.B, item.C);

        /* BEGIN CRITICAL REGION (Produção para o Buffer 1) */
        sem_wait(&shared[1].empty); // Espera por slot vazio
        sem_wait(&shared[1].mutex); // Espera por acesso exclusivo

            // 5. Produz o item processado para o Buffer 1
            shared[1].buf[shared[1].in] = item;
            shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
            printf("[CP1_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[1].mutex); // Libera o Mutex
        sem_post(&shared[1].full);  // Sinaliza que há um slot cheio
        /* END CRITICAL REGION */
    }
    return NULL;
}


//CP2 - Thread Consumidora & Produtora 2 Move shared[1]→buffer[out] para um ponteiro temporário,
//calcula V como a soma das colunas de C. (teremos 4 instâncias desta thread)
void *ConsumerProducer2(void *arg)
{
    int index;
    Data item;
    
    index = *((int *)arg);
    printf("Starting CP2 thread %d...\n", index);
    while(1)
    {
        //int consumed = 0;
        // BEGIN CRITICAL REGION
        printSharedBuffer(&shared[2], "[CP2]");
        sem_wait(&shared[1].full);
        sem_wait(&shared[1].mutex);

            item = shared[1].buf[shared[1].out];
            shared[1].out = (shared[1].out + 1) % BUFF_SIZE;
            //consumed = shared[1].buf[shared[1].out].consumed;
            printf("[CP2_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);
            
        sem_post(&shared[1].mutex);
        sem_post(&shared[1].empty);
        // END CRITICAL REGION
        if (item.kill == KILL) {
           
           pthread_mutex_lock(&kill_mutex_NCP3);
           
           if (kill_propagated_NCP3 == 0) {
               kill_threads_NCP3(); 
               kill_propagated_NCP3 = 1;
               printf("[CP1_LIDER] Propagando KILL para CP2.\n");
           }
           
           pthread_mutex_unlock(&kill_mutex_NCP3);

           printf("[CP1_%d] Consumed KILL and terminating.\n", index); 
           fflush(stdout);
           return NULL; // Termina a thread CP1
        }

        sumColumns(item.C, item.V);
        
        // BEGIN CRITICAL REGION
        sem_wait(&shared[2].empty);
        sem_wait(&shared[2].mutex);

            shared[2].buf[shared[2].in] = item;
            shared[2].in = (shared[2].in + 1) % BUFF_SIZE;
            printf("[CP2_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[2].mutex);
        sem_post(&shared[2].full);
        // END CRITICAL REGION
        //free(ptr);
    }
    return NULL;
}


//CP3 - Thread Consumidora & Produtora 3 Move shared[2]→buffer[out] para um ponteiro temporário,
//calcula E como a soma dos elementos de V. Move o ponteiro temporário para shared[3]→buffer[in). Teremos 2 instâncias desta thread.

void *ConsumerProducer3(void *arg)
{
    int index;
    Data item;
    index = *((int *)arg);
    
    while(1)
    {
        //BEGIN CRITICAL REGION
        printSharedBuffer(&shared[3], "[CP3]");
        sem_wait(&shared[2].full);
        sem_wait(&shared[2].mutex);

            item = shared[2].buf[shared[2].out];
            shared[2].out = (shared[2].out + 1) % BUFF_SIZE;
            printf("[CP3_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[2].mutex);
        sem_post(&shared[2].empty);
        //END CRITICAL REGION
        
        if (item.kill == KILL) {
            
            pthread_mutex_lock(&kill_mutex_NC);
            
            if (kill_propagated_NC == 0) {
               kill_threads_NC(); // Chamada corrigida (NC * 1)
               kill_propagated_NC = 1;
               printf("[CP3_LIDER] Propagando KILL para Consumidores.\n");
            }
            
            pthread_mutex_unlock(&kill_mutex_NC);

            printf("[CP3_%d] Received kill message and terminating.\n", index); fflush(stdout);
            return NULL;
        }

        sumV(item.V, &item.E);

        //BEGIN CRITICAL REGION
        sem_wait(&shared[3].empty);
        sem_wait(&shared[3].mutex);

            shared[3].buf[shared[3].in] = item;
            shared[3].in = (shared[3].in + 1) % BUFF_SIZE;
            printf("[CP3_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[3].mutex);
        sem_post(&shared[3].full);
        //END CRITICAL REGION
    }
    return NULL;
}

// void *ConsumerProducer3(void *arg)
// {
//     int index;
//     Data item;
//     index = *((int *)arg);
    
//     while(1)
//     {
//         //BEGIN CRITICAL REGION
//         //int consumed = 0;
//         sem_wait(&shared[2].full);
//         sem_wait(&shared[2].mutex);

//             item = shared[2].buf[shared[2].out];
//             //Data *ptr = (Data *)createCPPointer(&item);
//             shared[2].out = (shared[2].out + 1) % BUFF_SIZE;
//             //consumed = shared[2].buf[shared[2].out].consumed;
//             shared[2].buf[shared[2].out].consumed = 1;
//             printf("[CP3_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
//             fflush(stdout);

//         sem_post(&shared[2].mutex);
//         sem_post(&shared[2].empty);
//         //END CRITICAL REGION
//         if (item.kill == KILL) {
//             printf("[CP3_%d] Received kill message\n", index); fflush(stdout);
//             kill_threads_NC();
//             break;
//         }

//         sumV(item.V, &item.E);

//         //BEGIN CRITICAL REGION
//         sem_wait(&shared[3].empty);
//         sem_wait(&shared[3].mutex);

//             shared[3].buf[shared[3].in] = item;
//             shared[3].in = (shared[3].in + 1) % BUFF_SIZE;
//             printf("[CP3_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
//             fflush(stdout);

//         sem_post(&shared[3].mutex);
//         sem_post(&shared[3].full);
//         //END CRITICAL REGION
//     }
//     return NULL;
// }
