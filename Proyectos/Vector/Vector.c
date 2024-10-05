#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Vector.h"


bool redimensionarVector(Vector* vector, float factor);
void ordenarBurbujeo(Vector* vector);
void ordenarSeleccion(Vector* vector);
void ordenarInsercion(Vector* vector);
void intercambiar(int* a, int* b);
int* buscarMenor(int* ini, int* fin);


// Primitivas

bool vectorCrear(Vector* vector)
{
    vector->ce = 0;
    vector->vec = malloc(CAP_INI * sizeof(int));

    if(vector->vec == NULL)
    {
        vector->cap = 0;
        return false;
    }

    vector->cap = CAP_INI;

    return true;
}


int vectorOrdInsertar(Vector* vector, int elem)
{
    if(vector->ce == vector->cap)
    {
        if(!redimensionarVector(vector, FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    int* posIns = vector->vec;
    int* ult = vector->vec + vector->ce - 1;

    while(posIns <= ult && elem > *posIns)
    {
        posIns++;
    }

    if(posIns <= ult && elem == *posIns)
    {
        return DUPLICADO;
    }

    for(int* i = ult; i >= posIns; i--)
    {
       *(i + 1) = *i;
    }

    *posIns = elem;

    vector->ce++;

    return TODO_OK;
}


int vectorInsertarAlFinal(Vector* vector, int elem)
{
    if(vector->ce == vector->cap)
    {
        if(!redimensionarVector(vector, FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    int* ult = vector->vec + vector->ce - 1;
    int* posIns = ult + 1;
    *posIns = elem;
    vector->ce++;
    return TODO_OK;
}


int vectorOrdBuscar(const Vector* vector, int elem)
{
    int* li = vector->vec;
    int* ls = vector->vec + vector->ce - 1;
    int* m;
    int comp;

    while(li < ls)
    {
        m = li + (ls - li) / 2;
        comp = elem - *m;

        if(comp == 0)
        {
            return m - vector->vec;
        }

        if(comp < 0)
        {
            ls = m - 1;
        }

        if(comp > 0)
        {
            li = m + 1;
        }
    }

    return -1;
}


bool vectorOrdEliminarElem(Vector* vector, int elem)
{
    if(vector->ce == 0)
    {
        return false;
    }

    int* posElim = vector->vec;
    int* ult = vector->vec + vector->ce - 1;

    while(posElim <= ult && elem > *posElim)
    {
        posElim++;
    }

    if(posElim > ult || elem < *posElim)
    {
        return false;
    }

    for(int* i = posElim; i < ult; i++)
    {
        *i = *(i + 1);
    }

    vector->ce--;

    if((float)vector->ce / vector->cap <= FACTOR_OCUP)
    {
        redimensionarVector(vector, FACTOR_DECR);
    }

    return true;
}


void vectorEliminar(Vector* vector)
{
    free(vector->vec);
    vector->vec = NULL;
    vector->ce = 0;
    vector->cap = 0;
}


void vectorVaciar(Vector* vector)
{
    realloc(vector->vec, CAP_INI * sizeof(int));
    vector->ce = 0;
    vector->cap = CAP_INI;
}


void vectorMostrar(const Vector* vector)
{
    int* ult = vector->vec + vector->ce - 1;

    for(int* i = vector->vec; i <= ult; i++)
    {
        printf("[%Id]: %d\n", i - vector->vec, *i);
    }
}


// Funciones privadas

bool redimensionarVector(Vector* vector, float factor)
{
    size_t nCap = vector->cap * factor;
    
    if(nCap < CAP_INI)
    {
        return false;
    }
    
    int* nVec = realloc(vector->vec, nCap * sizeof(int));

    if(!nVec)
    {
        return false;
    }

    vector->vec = nVec;

    printf("Redimension de %Iu a %Iu\n", vector->cap, nCap);

    vector->cap = nCap;

    return true;
}


void cargarVectorRandom(Vector* vector, int ce)
{
    srand(time(NULL));

    for(int i = 0; i < ce; i++)
    {
        vectorInsertarAlFinal(vector, rand() % 100);
    }
}


void vectorOrdenar(Vector* vector, int metodo)
{
    switch(metodo)
    {
        case BURBUJEO:
            ordenarBurbujeo(vector);
            break;
        
        case SELECCION:
            ordenarSeleccion(vector);
            break;
        
        case INSERCION:
            ordenarInsercion(vector);
            break;
    }
}


void ordenarBurbujeo(Vector* vector)
{
    int* ult = vector->vec + vector->ce - 1;

    for(int i = 1, *limJ = ult - 1; i < vector->ce; i++, limJ--)
    {
        for(int* j = vector->vec; j <= limJ; j++)
        {
            if(*j > *(j + 1))
            {
                intercambiar(j, j + 1);
            }
        }
    }
}


void ordenarSeleccion(Vector* vector)
{
    int* ult = vector->vec + vector->ce - 1;
    int* m = NULL;

    for(int* i = vector->vec; i < ult; i++)
    {
        m = buscarMenor(i, ult);
        intercambiar(m, i);
    }
}


int* buscarMenor(int* ini, int* fin)
{
    int* m = ini;

    for(int* j = ini + 1; j <= fin; j++)
    {
        if(*j < *m)
        {
            m = j;
        }
    }

    return m;
}


void ordenarInsercion(Vector* vector)
{
    int* ult = vector->vec + vector->ce - 1;
    int elemAIns;

    for(int* i = vector->vec + 1; i <= ult; i++)
    {
        elemAIns = *i;
        
        int* j;
        for(j = i - 1; j >= vector->vec && elemAIns < *j; j--)
        {
            *(j + 1) = *j;
        }

        *(j + 1) = elemAIns;
    }
}


void intercambiar(int* a, int* b)
{
    int aTemp = *a;
    *a = *b;
    *b = aTemp;
}


