#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Vector.h"


bool redimensionarVector(Vector* vector, float factor);
void ordenarBurbujeo(Vector* vector);
void ordenarSeleccion(Vector* vector, Cmp cmp);
void ordenarInsercion(Vector* vector);
void intercambiar(void* a, void* b, size_t tamElem);
const void* buscarMenor(const void* ini, const void* fin, size_t tamElem, Cmp cmp);


// Primitivas

bool vectorCrear(Vector* vector, size_t tamElem)
{
    vector->ce = 0;
    vector->vec = malloc(CAP_INI * tamElem);

    if(vector->vec == NULL)
    {
        vector->cap = 0;
        return false;
    }

    vector->cap = CAP_INI;
    vector->tamElem = tamElem;

    return true;
}


int vectorOrdInsertar(Vector* vector, const void* elem, Cmp cmp)
{
    if(vector->ce == vector->cap)
    {
        if(!redimensionarVector(vector, FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    void* posIns = vector->vec;
    void* ult = vector->vec + (vector->ce - 1) * vector->tamElem;

    while(posIns <= ult && cmp(elem, posIns) > 0)
    {
        posIns += vector->tamElem;
    }

    if(posIns <= ult && cmp(elem, posIns) == 0)
    {
        return DUPLICADO;
    }

    for(void* i = ult; i >= posIns; i -= vector->tamElem)
    {
       memcpy(i + vector->tamElem, i, vector->tamElem); // *(i + 1) = *i;
    }

    memcpy(posIns, elem, vector->tamElem);

    vector->ce++;

    return TODO_OK;
}


int vectorInsertarAlFinal(Vector* vector, const void* elem)
{
    if(vector->ce == vector->cap)
    {
        if(!redimensionarVector(vector, FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    void* ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void* posIns = ult + vector->tamElem;
    memcpy(posIns, elem, vector->tamElem);
    vector->ce++;
    return TODO_OK;
}


int vectorOrdBuscar(const Vector* vector, void* elem, Cmp cmp)
{
    void* li = vector->vec;
    void* ls = vector->vec + (vector->ce - 1) * vector->tamElem;
    void* m;
    int comp;

    while(li < ls)
    {
        m = li + ((ls - li) / vector->tamElem / 2) * vector->tamElem;
        comp = cmp(elem, m);

        if(comp == 0)
        {
            memcpy(elem, m, vector->tamElem);
            return (m - vector->vec) / vector->tamElem;
        }

        if(comp < 0)
        {
            ls = m - vector->tamElem;
        }

        if(comp > 0)
        {
            li = m + vector->tamElem;
        }
    }

    return -1;
}


bool vectorOrdEliminarElem(Vector* vector, void* elem, Cmp cmp)
{
    if(vector->ce == 0)
    {
        return false;
    }

    void* posElim = vector->vec;
    void* ult = vector->vec + (vector->ce - 1) * vector->tamElem;

    while(posElim <= ult && cmp(elem, posElim) > 0)
    {
        posElim += vector->tamElem;
    }

    if(posElim > ult || cmp(elem, posElim) < 0)
    {
        return false;
    }

    memcpy(elem, posElim, vector->tamElem);

    for(void* i = posElim; i < ult; i += vector->tamElem)
    {
        memcpy(i, i + vector->tamElem, vector->tamElem);
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
    vector->vec = realloc(vector->vec, CAP_INI * vector->tamElem);
    vector->ce = 0;
    vector->cap = CAP_INI;
}


// void vectorMostrar(const Vector* vector)
// {
//     int* ult = vector->vec + vector->ce - 1;

//     for(int* i = vector->vec; i <= ult; i++)
//     {
//         printf("[%Id]: %d\n", i - vector->vec, *i);
//     }
// }


void vectorRecorrer(const Vector* vector, Accion accion, void* datosA)
{
    void* ult = vector->vec + (vector->ce - 1) * vector->tamElem;

    for(void* i = vector->vec; i <= ult; i += vector->tamElem)
    {
        accion((i - vector->vec) / vector->tamElem, i, datosA);
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

    int* nVec = realloc(vector->vec, nCap * vector->tamElem);

    if(!nVec)
    {
        return false;
    }

    vector->vec = nVec;

    printf("Redimension de %llu a %llu\n", vector->cap, nCap);

    vector->cap = nCap;

    return true;
}


void vectorOrdenar(Vector* vector, int metodo, Cmp cmp)
{
    switch(metodo)
    {
//         case BURBUJEO:
//             ordenarBurbujeo(vector);
//             break;

        case SELECCION:
            ordenarSeleccion(vector, cmp);
            break;

//         case INSERCION:
//             ordenarInsercion(vector);
//             break;
    }
}


// void ordenarBurbujeo(Vector* vector)
// {
//     int* ult = vector->vec + vector->ce - 1;

//     for(int i = 1, *limJ = ult - 1; i < vector->ce; i++, limJ--)
//     {
//         for(int* j = vector->vec; j <= limJ; j++)
//         {
//             if(*j > *(j + 1))
//             {
//                 intercambiar(j, j + 1);
//             }
//         }
//     }
// }


void ordenarSeleccion(Vector* vector, Cmp cmp)
{
    void* ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void* m = NULL;

    for(void* i = vector->vec; i < ult; i += vector->tamElem)
    {
        m = (void*)buscarMenor(i, ult, vector->tamElem, cmp);
        intercambiar(m, i, vector->tamElem);
    }
}


const void* buscarMenor(const void* ini, const void* fin, size_t tamElem, Cmp cmp)
{
    const void* m = ini;

    for(const void* j = ini + tamElem; j <= fin; j += tamElem)
    {
        if(cmp(j, m) < 0)
        {
            m = j;
        }
    }

    return m;
}


// void ordenarInsercion(Vector* vector)
// {
//     int* ult = vector->vec + vector->ce - 1;
//     int elemAIns;

//     for(int* i = vector->vec + 1; i <= ult; i++)
//     {
//         elemAIns = *i;

//         int* j;
//         for(j = i - 1; j >= vector->vec && elemAIns < *j; j--)
//         {
//             *(j + 1) = *j;
//         }

//         *(j + 1) = elemAIns;
//     }
// }


void intercambiar(void* a, void* b, size_t tamElem)
{
    void* aTemp = malloc(tamElem);
    memcpy(aTemp, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aTemp, tamElem);
    free(aTemp);
}


