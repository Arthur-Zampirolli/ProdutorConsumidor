
#include "matrix.h"
void *matrixMultiply(double a[DIMENSION][DIMENSION], double b[DIMENSION][DIMENSION], double c[DIMENSION][DIMENSION])
{
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < DIMENSION; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
                /*Exemplo: c[0][0] = a[0][0]*b[0][0]+
                a[0][1]*b[1][0]+
                a[0][2]*b[2][0];
                */
            }
        }
    }
    return NULL;
}
void initMatrix(double matrix[DIMENSION][DIMENSION])
{
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            matrix[i][j] = 0.0;
        }
    }
}
void copyMatrix(double source[DIMENSION][DIMENSION], double destination[DIMENSION][DIMENSION]){
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            destination[i][j] = source[i][j];
        }
    }
}
void printMatrix(double matrix[DIMENSION][DIMENSION]){
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void sumColumns(double matrix[DIMENSION][DIMENSION], double vector[DIMENSION]){
    for (int j = 0; j < DIMENSION; j++)
    {
        vector[j] = 0.0;
        for (int i = 0; i < DIMENSION; i++)
        {
            vector[j] += matrix[i][j];
        }
    }
}
void sumV(double V[DIMENSION], double E){
    E = 0.0;
    for (int i = 0; i < DIMENSION; i++)
    {
        E += V[i];
    }
}