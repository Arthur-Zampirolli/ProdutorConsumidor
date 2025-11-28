#include "consumer.h"

#ifndef BUFFER_INCLUDED
#define BUFFER_INCLUDED
  #include "../data.h"
  #include "../data_buffer.h"
#endif


/*
C - Thread consumidora - Escreve um arquivo (saida.out) contendo Nome, A, B, C,
V e E para cada um dos arquivos de entrada, obedecendo o seguinte formato (substituir
<variável> pelo valor da variável). Teremos 1 instância desta thread: 
*/
void *Consumer(void *arg)
{
    int i, index;
    Data item;
    index = *((int *)arg);

    for (i = 0; i < NITERS; i++)
    {

        sem_wait(&shared[1].full);
        sem_wait(&shared[1].mutex);

          item = shared[1].buf[shared[1].out];
          char fileName[100];
          sprintf(fileName, "%ssaida%d.out", OUTPUT_PATH, i);
          saveData(&item, fileName);
          shared[1].out = (shared[1].out + 1) % BUFF_SIZE;
          printf("[C_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
          printf("[C_%d] Data saved to %s\n", index, fileName);
          fflush(stdout);

        sem_post(&shared[1].mutex);
        sem_post(&shared[1].empty);
    }
    return NULL;
}