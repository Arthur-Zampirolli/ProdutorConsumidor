
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