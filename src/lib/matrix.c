
#include "matrix.h"
#include <omp.h>


void *matrixMultiply(double a[DIMENSION][DIMENSION], double b[DIMENSION][DIMENSION], double c[DIMENSION][DIMENSION])
{
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < DIMENSION; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
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



void *sumColumns(double matrix[DIMENSION][DIMENSION], double *vector){
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int j = 0; j < DIMENSION; j++)
    {
        vector[j] = 0.0;
        for (int i = 0; i < DIMENSION; i++)
        {
            vector[j] += matrix[i][j];
        }
    }

    return vector;
}
void sumV(double V[DIMENSION], double *E){
    int r = 0.0;
    for (int i = 0; i < DIMENSION; i++)
    {
        r += V[i];
    }
    *E = r;
}