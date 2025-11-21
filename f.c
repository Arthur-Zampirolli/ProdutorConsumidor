#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>

#define BUFF_SIZE 10 /* total number of slots */
#define NP 1        /* total number of producers */
#define NCP 3       /* total number of consumers/producers */
#define NC 1        /* total number of consumers */
#define NITERS 50    /* number of items produced/consumed */
#define KILL -1
typedef struct
{
    int buf[BUFF_SIZE]; /* shared var */
    int in;             /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;            /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;         /* keep track of the number of full spots */
    sem_t empty;        /* keep track of the number of empty spots */
    sem_t mutex;        /* enforce mutual exclusion to shared data */
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

        sem_wait(&shared[0].empty);
        sem_wait(&shared[0].mutex);

        shared[0].buf[shared[0].in] = item;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
        printf("[P_%d] Producing %d ...\n", index, item);
        fflush(stdout);
        
        sem_post(&shared[0].mutex);
        sem_post(&shared[0].full);
    }
    printf("sending KILL signal for all CP's...\n");
    for (int j = 0; j < NCP; j++)
    {
        sem_wait(&shared[0].empty);
        /* If another thread uses the buffer, wait */
        sem_wait(&shared[0].mutex);

        printf("Sending kill signal for CP%d\n", j);
        shared[0].buf[shared[0].in] = KILL;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
        /* Release the buffer */
        sem_post(&shared[0].mutex);
        /* Increment the number of full slots */
        sem_post(&shared[0].full);
    }

    printf("Fim produtor\n");
    return NULL;
}

void *ConsumerProducer(void *arg)
{
    int i, item, index;

    index = *((int *)arg);

    while(1)
    {
        sem_wait(&shared[0].full);
        sem_wait(&shared[0].mutex);

        item = shared[0].buf[shared[0].out];
        shared[0].out = (shared[0].out + 1) % BUFF_SIZE;
        
        if (item == KILL)
        {
            printf("killing CP_%d...\n", index);
            printf("Sending kill signal for consumers...\n");


            sem_post(&shared[0].mutex);
            sem_post(&shared[0].full);
            for (int j = 0; j < NC; j++)
            {
                sem_wait(&shared[1].empty);
                sem_wait(&shared[1].mutex);


                shared[1].buf[shared[1].in] = KILL;
                shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
                fflush(stdout);

                sem_post(&shared[1].mutex);
                sem_post(&shared[1].full);
            }

            break;
        }
        printf("[CP1_%d] Consuming %d ...\n", index, item);
        fflush(stdout);

        sem_post(&shared[0].mutex);
        sem_post(&shared[0].empty);

        item += 500;

        sem_wait(&shared[1].empty);
        sem_wait(&shared[1].mutex);

        shared[1].buf[shared[1].in] = item;
        shared[1].in = (shared[1].in + 1) % BUFF_SIZE;
        printf("[CP1_%d] Producing %d ...\n", index, item);
        fflush(stdout);

        sem_post(&shared[1].mutex);
        sem_post(&shared[1].full);
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, item, index;

    index = *((int *)arg);

    while(1)
    {

        sem_wait(&shared[1].full);
        sem_wait(&shared[1].mutex);

        item = shared[1].buf[shared[1].out];
        shared[1].out = (shared[1].out + 1) % BUFF_SIZE;

        if (item == KILL)
        {
            printf("Killing consumer %d\n", index);
            sem_post(&shared[1].mutex);
            sem_post(&shared[1].empty);
            break;
        }
        printf("[C_%d] Consuming %d ...\n", index, item);
        fflush(stdout);

        sem_post(&shared[1].mutex);
        sem_post(&shared[1].empty);
    }
    return NULL;
}

int main()
{
    pthread_t idP[NP], idC[NC], idCP[NCP];
    int index;
    int sP[NP], sC[NC], sCP[NCP];


    for (index = 0; index < 2; index++)
    {
        sem_init(&shared[index].full, 0, 0);
        sem_init(&shared[index].empty, 0, BUFF_SIZE);
        sem_init(&shared[index].mutex, 0, 1);

                // Inicializa in/out
        shared[index].in = 0;
        shared[index].out = 0;
    }

    for (index = 0; index < NP; index++)
    {
        sP[index] = index;
        /* Create a new producer */
        pthread_create(&idP[index], NULL, Producer, &sP[index]);
    }

    for (index = 0; index < NCP; index++)
    {
        sCP[index] = index;
        /* Create a new producer */
        pthread_create(&idCP[index], NULL, ConsumerProducer, &sCP[index]);
    }

    for (index = 0; index < NC; index++)
    {
        sC[index] = index;
        /* Create a new consumer */
        pthread_create(&idC[index], NULL, Consumer, &sC[index]);
    }
    for (index = 0; index < NCP; index++)
    {
        pthread_join(idP[index], NULL);
    }
    for (index = 0; index < NCP; index++)
    {
        pthread_join(idCP[index], NULL);
    }
    for (index = 0; index < NC; index++)
    {
        pthread_join(idC[index], NULL);
    }
    

    for (index = 0; index < 2; index++)
    {
        sem_destroy(&shared[index].full);
        sem_destroy(&shared[index].empty);
        sem_destroy(&shared[index].mutex);
    }

    printf("Programa encerrado com sucesso.\n");
    return 0;
}
