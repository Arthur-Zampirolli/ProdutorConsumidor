void *matrixMultiply(double **a, double **b, double **c)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 10; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
                /*Exemplo: c[0][0] = a[0][0]*b[0][0]+
                a[0][1]*b[1][0]+
                a[0][2]*b[2][0];

                */
            }
        }
    }
}