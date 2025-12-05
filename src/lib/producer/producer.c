#include "producer.h"
#include "../io.h"
#ifndef BUFFER_INCLUDED
#define BUFFER_INCLUDED
  #include "../data.h"
#endif

int fileLines = 0;

//is going to read the file and alocates the matrices in shared memory
Data *loadFromFile(char* filename){
    Data *data = malloc(sizeof(Data));
    data->kill = 0;
    loadMatrices(filename, data->A, data->B);
    strcpy(data->fileA, filename);
    strcpy(data->fileB, filename);
    data->E = 0.0;
    data->consumed = 0;
    memset(data->V, 0, sizeof(double)*DIMENSION);
    initMatrix(data->C);
    return data;
}

int countFileLines(char *inputFile) {
    printf("Counting lines in file %s...\n", inputFile);
    FILE *fp = fopen(inputFile, "r");
    if (!fp) {
        perror("File error:\n");
        return -1;
    }

    char buffer[STRING_MAX];
    int lineCount = 0;
    while (fgets(buffer, STRING_MAX, fp) != NULL) {
        int size = strlen(buffer);
        if (size > 1) { // skip empty lines
            lineCount++;
        }
    }

    fclose(fp);
    return lineCount;
}

char **getFileNames(char *inputFile, int *numFiles) {

    printf("Reading file names from %s...\n", inputFile);
    FILE *fp = fopen(inputFile, "r");
    if (!fp) {
        perror("File error:\n");
        return NULL;
    }

    char **fileNames = malloc(sizeof(char *) * countFileLines(inputFile));
    char buffer[STRING_MAX+6];
    *numFiles = 0;
    while (fgets(buffer, STRING_MAX, fp) != NULL) {
        int size = strlen(buffer);
        printf("Found file: %s\n", buffer);
        if (size <= 1) {
            continue; // skip empty lines
        }
        buffer[size - 1] = '\0'; // remove newline character
        fileNames[*numFiles] = malloc(strlen(buffer) + 1);
        strcpy(fileNames[*numFiles], buffer);
        filesTotal++;
        (*numFiles)++;
    }

    fclose(fp);
    return fileNames;
}

void killThreadsCP1() {
  for (int i = 0; i < NCP1*2; i++) {
    Data *kill_item = (Data *) malloc(sizeof(Data));
    kill_item->kill = KILL;
    //BEGIN CRITICAL REGION
    sem_wait(&shared[0].empty);
    sem_wait(&shared[0].mutex);

        shared[0].buf[shared[0].in] = *kill_item;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;

    sem_post(&shared[0].full);
    sem_post(&shared[0].mutex);
    //END CRITICAL REGION
    free(kill_item);
  }
}
void *Producer(void *arg)
{
    printf("Starting Producer thread...\n");
    int i, index;
    Data item;
    index = *((int *)arg);
    
    char **fileNames = getFileNames(INPUT_FILE, &fileLines);
    printf("Producer %d found %d files to process.\n", index, fileLines);
    for (i = 0; i < fileLines; i++)
    {   
        printSharedBuffer(&shared[0], "[P]");
        sem_wait(&shared[0].empty);
        sem_wait(&shared[0].mutex);

            Data *data = loadFromFile(fileNames[i]);
            item = *data;
            shared[0].buf[shared[0].in] = *data;
            shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
            printf("[P_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[0].mutex);
        sem_post(&shared[0].full);
        free(data);
    }
    killThreadsCP1();
    printf("End producer\n");
    return NULL;
}