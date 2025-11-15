#ifdef IO
#define IO
#include "constants.h"
void printmatrix(double m[DIMENSION][DIMENSION]);
void linecpy(double a[DIMENSION], double b[DIMENSION]);
void loadMatrices(char *filename, double A[DIMENSION][DIMENSION], double B[DIMENSION][DIMENSION]);

#endif