#ifndef MATRIX
#define MATRIX
#define DIMENSION 10

#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

double **loadMAtrixFromFile(char *filePath);
void saveMatrixToFile(char *filePath, double **matrix);
void *matrixMultiply(double a[DIMENSION][DIMENSION], double b[DIMENSION][DIMENSION], double c[DIMENSION][DIMENSION]);
void initMatrix(double matrix[DIMENSION][DIMENSION]);
#endif