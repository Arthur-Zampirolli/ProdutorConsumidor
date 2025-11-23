#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

void printmatrix(double m[DIMENSION][DIMENSION]);
void linecpy(double a[DIMENSION], double b[DIMENSION]);
void loadMatrices(char *filename, double A[DIMENSION][DIMENSION], double B[DIMENSION][DIMENSION]);
int saveMatrix(char *filename, double A[DIMENSION][DIMENSION]);
#endif