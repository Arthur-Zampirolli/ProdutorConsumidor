#ifndef MATRIX
#define MATRIX
#define DIMENSION 10


double **loadMAtrixFromFile(char *filePath);
void saveMatrixToFile(char *filePath, double **matrix);
void *matrixMultiply(double a[DIMENSION][DIMENSION], double b[DIMENSION][DIMENSION], double c[DIMENSION][DIMENSION]);

#endif