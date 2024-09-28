#include <stdio.h>
#include <stdlib.h>
#include "Matriz.h"


void** crearMatriz(int fil, int col, size_t tamElem)
{
    void** mat = malloc(sizeof(void*) * fil);

    if(!mat)
    {
        return NULL;
    }

    void** ult = mat + fil - 1;

    for(void** i = mat; i <= ult; i++)
    {
        *i = malloc(tamElem * col);

        if(!*i)
        {
            destruirMatriz(mat, i - mat);
            return NULL;
        }
    }

    return mat;
}


void destruirMatriz(void** mat, int fil)
{
    void** ult = mat + fil - 1;

    for(void** i = mat; i <= ult; i++)
    {
        free(*i);
    }

    free(mat);
}


void cargarMatriz(int fil, int col, int** mat)
{
    int cont = 1;

    for(int i = 0; i < fil; i++)
    {
        for(int j = 0; j < col; j++)
        {
            mat[i][j] = cont++;
        }
    }    
}


void mostrarMatriz(int fil, int col, int** mat)
{
    for(int i = 0; i < fil; i++)
    {
        for(int j = 0; j < col; j++)
        {
            printf("%3d", mat[i][j]);
        }
        putchar('\n');
    }
}


int sumaDiagonalPrincipal(int orden, int** mat)
{
    int acum = 0;

    for(int i = 0; i < orden; i++)
    {
        acum += mat[i][i];
    }

    return acum;
}


int sumaDiagonalSecundaria(int orden, int** mat)
{
    int acum = 0;

    for(int i = 0, j = orden - 1; i < orden; i++, j--)
    {
        acum += mat[i][j];
    }

    return acum;
}


int sumaTriangInfDP(int orden, int** mat)
{
    int acum = 0;

    for(int i = 1; i < orden; i++)
    {
        for(int j = 0; j < i; j++)
        {
            acum += mat[i][j];
        }
    }

    return acum;
}



int sumaTriangSupDS(int orden, int** mat)
{
    int acum = 0;
    int limI = orden - 2;

    for(int i = 0, limJ = orden - 2; i <= limI; i++, limJ--)
    {
        for(int j = 0; j <= limJ; j++)
        {
            acum += mat[i][j];
        }
    }

    return acum;
}
