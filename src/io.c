#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "io.h"

void printmatrix(double m[DIMENSION][DIMENSION]){
    for(int i = 0; i < DIMENSION; i++){
        for(int j = 0; j < DIMENSION; j++){
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
}

void linecpy(double a[DIMENSION], double b[DIMENSION]){
    for(int i = 0; i < DIMENSION; i++){
        a[i] = b[i];
    }
    return;
}

void loadMatrices(char *filename, double A[DIMENSION][DIMENSION], double B[DIMENSION][DIMENSION]){
    FILE *input = fopen(filename, "r");
    if(!input){
        perror("Error opening file");
        return;
    }
    char * buffer = malloc(sizeof(char)*STRING_MAX);
    double line[DIMENSION];
    int i = 0;
    int j = 0;
    int second = 0;
    while(fgets(buffer, STRING_MAX, input) != NULL){
        if(strlen(buffer) <= 1){

            second = 1;
        }
        if(strlen(buffer)>1){
            sscanf(buffer, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &line[0], &line[1], &line[2], &line[3], &line[4], &line[5], &line[6], &line[7], &line[8], &line[9]);
            if(second == 0){
                linecpy(A[i],line);  
                i++;
            }
            else{
                linecpy(B[j], line);
                j++;
            }
        }
    }
    fclose(input);
    return;
}

