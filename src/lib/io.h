#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "data.h"

void printmatrix(double m[DIMENSION][DIMENSION]);
void linecpy(double a[DIMENSION], double b[DIMENSION]);
void loadMatrices(char *filename, double A[DIMENSION][DIMENSION], double B[DIMENSION][DIMENSION]);
int saveMatrix(char *filename, double A[DIMENSION][DIMENSION]);
void saveData(Data* data, const char* outputFile);
#endif