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


// int reader(){
//     // {
// //     // int size = 50;
// //     double A[DIMENSION][DIMENSION];
// //     double B[DIMENSION][DIMENSION];
// //     double C[DIMENSION][DIMENSION];
// //     char fileInput[STRING_MAX + 6]; // gambiarra do bem

// //     FILE *fp = fopen("./input/entrada.in", "r");
// //     if (!fp)
// //     {
// //         perror("File error:\n");
// //         return 1;
// //     }
// //     char buffer[STRING_MAX];
// //     while (fgets(buffer, STRING_MAX, fp) != NULL)
// //     {
// //         int size = strlen(buffer);
// //         buffer[size - 1] = '\0';
// //         if (size <= 1)
// //         {
// //             // EOF
// //             continue;
// //         }
// //         if (size > 1)
// //         {

// //             sprintf(fileInput, "%s", buffer);

// //             // fileInput[size - 1] = '\0'

// //             printf("Loading file: %s\n", fileInput);
// //             loadMatrices(fileInput, A, B);
// //             matrixMultiply(A, B, C);
// //             printf("----------MATRIX_A------------\n");
// //             printmatrix(A);
// //             printf("----------MATRIX_B------------\n");
// //             printmatrix(B);
// //             printf("-----------RESULT-------------\n");
// //             printmatrix(C);
// //         }
// //     }
// //     saveMatrix("./output/result.out", C);
// //     fclose(fp);
// //     return 0;
// }

char **getFileNames(char *inputFile, int *numFiles) {
    printf("Reading file names from %s...\n", inputFile);
    FILE *fp = fopen(inputFile, "r");
    if (!fp) {
        perror("File error:\n");
        return NULL;
    }

    char **fileNames = malloc(sizeof(char *) * 100); // assuming max 100 files
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
        
        (*numFiles)++;
    }

    fclose(fp);
    return fileNames;
}

void kill_threads_NCP1() {
  for (int i = 0; i < NCP1*2; i++) {
    Data *kill_item = (Data *) malloc(sizeof(Data));
    kill_item->kill = KILL;

    sem_wait(&shared[0].empty);
    sem_wait(&shared[0].mutex);

        shared[0].buf[shared[0].in] = *kill_item;
        shared[0].in = (shared[0].in + 1) % BUFF_SIZE;

    sem_post(&shared[0].full);
    sem_post(&shared[0].mutex);
    
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

        sem_wait(&shared[0].empty);
        sem_wait(&shared[0].mutex);

            Data *data = loadFromFile(fileNames[i]);
            item = *data;
            shared[0].buf[shared[0].in] = *data;
            //shared[0].buf[shared[0].in].consumed = 0;
            shared[0].in = (shared[0].in + 1) % BUFF_SIZE;
            printf("[P_%d] Producing %s %s...\n", index, item.fileA, item.fileB);
            fflush(stdout);

        sem_post(&shared[0].mutex);
        sem_post(&shared[0].full);
        free(data);
    }
    kill_threads_NCP1();
    printf("Fim produtor\n");
    return NULL;
}