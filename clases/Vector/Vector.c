
#include <stdlib.h>
#include <stdio.h>
#include "Vector.h"

bool redimensionarVector(Vector* vector, float factor);


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

    for(int* i = ult; i>= posIns; i--)
    {
        *(i+1)=*i;
    }

    *posIns=elem;

    vector->ce++;

    return TODO_OK;
}


bool vectorOrdEliminarElem(Vector* vector, int elem)
{
    if(vector->ce == 0)
    {
        return false;
    }

    int* posElim = vector->vec;
    int* ult = vector->vec + vector->ce -1;

    while(posElim<=ult && elem > *posElim)
    {
       posElim++;
    }

    if(posElim > ult || elem < *posElim)
    {
        return false;
    }

    for(int* i = posElim; i<ult; i++)
    {
        *i = *(i+1);
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
    int* ult = vector->vec + vector->ce -1;
    for(int* i = vector->vec; i <= ult; i++)
    {
        printf("[%Iu]: %d\n", i - vector->vec, *i);
    }
}

//funcion privada

bool redimensionarVector(Vector* vector, float factor)
{
    size_t nCap = vector->cap * factor;
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
