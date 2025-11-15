#ifdef MATRIX
#define MATRIX
#define DIMENSION 10


double **loadMAtrixFromFile(char *filePath);
void saveMatrixToFile(char *filePath, double **matrix);
void *matrixMultiply(double **a, double **b, double **c);

#endif MATRIX