#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/semaphore.h>
#include <string.h>
#include "lib/constants.h"
//#include "lib/semaphore.h"
#include "lib/io.h"
#include "lib/matrix.h"
#include "lib/threads.h"
#include "lib/data_buffer.h"

S shared[5];


int main(){
    //int size = 50;
    double A[DIMENSION][DIMENSION];
    double B[DIMENSION][DIMENSION];
    double C[DIMENSION][DIMENSION];
    char fileInput[STRING_MAX+6];//gambiarra do bem
    
    FILE *fp = fopen("./input/entrada.in", "r");
    if(!fp){
        perror("File error:\n");
        return 1;
    }
    char buffer[STRING_MAX];
    while(fgets(buffer, STRING_MAX, fp) != NULL){
        int size = strlen(buffer);
        buffer[size - 1] = '\0';
        if(size <= 1){
            //EOF
            continue;
        }
        if(size > 1){
            
            sprintf(fileInput, "%s", buffer);
            
            //fileInput[size - 1] = '\0'
            
            printf("Loading file: %s\n", fileInput);
            loadMatrices(fileInput, A, B);
            matrixMultiply(A, B, C);
            printf("----------MATRIX_A------------\n");
            printmatrix(A);
            printf("----------MATRIX_B------------\n");
            printmatrix(B);
            printf("-----------RESULT-------------\n");
            printmatrix(C);
            
        }

    }
    saveMatrix("./output/result.out", C);
    fclose(fp);
    return 0;
}


