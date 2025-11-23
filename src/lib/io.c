
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
// para resolver o problema, pediu-se dimensão 10, então vou fazer hardcoded pra 10
int saveMatrix(char *filename, double A[DIMENSION][DIMENSION]){
    FILE * fp = fopen(filename, "w");
    char result[STRING_MAX];
    if(!fp){
        perror("Unable to save the file");
        return 1;
    }
    for(int i = 0; i < DIMENSION; i++){
        char line[STRING_MAX];
        line[0] = '\0';
        for(int j = 0; j < DIMENSION; j++){
            char temp[50];
            sprintf(temp, "%lf", A[i][j]);
            if(j < DIMENSION - 1){
                strcat(temp, " ");
            }
            strcat(line, temp);
        }
        strcat(line, "\n");
        strcat(result, line);
    }
    int size = strlen(result);
    fwrite(result, sizeof(char), size-1, fp);
    fclose(fp);
    return 0;
}
