#include "Vector.h"

bool vectorCrear(Vector* vector)
{
    vector->ce = 0;
    return true;
}

int vectorOrdInsertar(Vector* vector, int elem)
{
    if(vector->ce == TAM_VEC)
    {
        return VEC_LLENO;
    }

    int* posIns = vector->vec;
    int* ult = vector->vec + vector->ce -1;

    while(posIns<=ult && elem > *posIns)
    {
        posIns++;
    }

    if(posIns<=ult && elem == *posIns)
    {
        return DUPLICADO;
    }

    for(int* i = ult; i>=posIns; i--)
    {
        *(i+1) = *i;
    }

    *posIns = elem;
    vector->ce++;

    return TODO_OK;

}

int vectorOrdBuscar(const Vector* vector, int elem)
{
    int* li = vector->vec;
    int* ls = vector->vec + vector->ce -1;
    int* m;
    int comp;

    while(li < ls)
    {
        m = li + (ls - li)/2;
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
    int* ult = vector->vec + vector->ce -1;

    while(posElim<=ult && elem > *posElim)
    {
        posElim++;
    }

    if(posElim > ult || elem < *posElim)
    {
        return false;
    }

    for(int* i = posElim; i < ult; i++)
    {
        *i = *(i+1);
    }

     vector->ce--; 

    return true;
}

void vectorEliminar(Vector* vec)
{

}

void vectorVaciar(Vector* vec)
{

}
