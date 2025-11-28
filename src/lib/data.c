#include "constants.h"
#include "data.h"


Data *initializeData(){
    Data *data = (Data *)malloc(sizeof(Data));
    if(data == 0){
        perror("Failed to allocate memory for Data\n");
        exit(EXIT_FAILURE);
    }
    data->kill = 0;
    data->E = 0.0;
    memset(data->fileA, 0, NAME_MAX);
    memset(data->fileB, 0, NAME_MAX);
    memset(data->A, 0, sizeof(double) * DIMENSION * DIMENSION);
    memset(data->B, 0, sizeof(double) * DIMENSION * DIMENSION);
    memset(data->C, 0, sizeof(double) * DIMENSION * DIMENSION);
    memset(data->V, 0, sizeof(double) * 10);
    return data;
}

void showData(Data *data){
    printf("Data contents:\n");
    printf("fileA: %s\n", data->fileA);
    printf("fileB: %s\n", data->fileB);
    printf("Matrix A:\n");
    for(int i = 0; i < DIMENSION; i++){
        for(int j = 0; j < DIMENSION; j++){
            printf("%lf ", data->A[i][j]);
        }
        printf("\n");
    }
    printf("Matrix B:\n");
    for(int i = 0; i < DIMENSION; i++){
        for(int j = 0; j < DIMENSION; j++){
            printf("%lf ", data->B[i][j]);
        }
        printf("\n");
    }
    printf("Matrix C:\n");
    for(int i = 0; i < DIMENSION; i++){
        for(int j = 0; j < DIMENSION; j++){
            printf("%lf ", data->C[i][j]);
        }
        printf("\n");
    }
    printf("Vector V:\n");
    for(int i = 0; i < 10; i++){
        printf("%lf ", data->V[i]);
    }
    printf("\nE: %lf\n", data->E);
}