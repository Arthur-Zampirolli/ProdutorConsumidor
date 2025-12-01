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

    for (i = 0; i < filesTotal; i++)
    {
        //int consumed = 0;
        printSharedBuffer(&shared[3]);
        sem_wait(&shared[3].full);
        sem_wait(&shared[3].mutex);

          item = shared[3].buf[shared[3].out];
          //consumed = shared[1].buf[shared[1].out].consumed;

          char fileName[100];
          if (item.kill == KILL) {
            printf("[C_%d] received KILL signal\n", index); fflush(stdout);
            return NULL;
          }

          sprintf(fileName, "%ssaida%d.out", OUTPUT_PATH, i);
          saveData(&item, fileName);
          shared[3].out = (shared[3].out + 1) % BUFF_SIZE;
          printf("[C_%d] Consuming %s %s...\n", index, item.fileA, item.fileB);
          printf("[C_%d] Data saved to %s\n", index, fileName);
          fflush(stdout);

        sem_post(&shared[3].mutex);
        sem_post(&shared[3].empty);
    }
    return NULL;
}