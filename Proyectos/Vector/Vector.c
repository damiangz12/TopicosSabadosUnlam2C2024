#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Vector.h"

bool redimensionarVector(Vector *vector, float factor);
void ordenarBurbujeo(Vector *vector, Cmp cmp);
void ordenarSeleccion(Vector *vector, Cmp cmp);
void ordenarInsercion(Vector *vector, Cmp cmp);
void intercambiar(void *a, void *b, size_t tamElem);
const void *buscarMenor(const void *ini, const void *fin, size_t tamElem, Cmp cmp);

// Primitivas

bool vectorCrear(Vector *vector, size_t tamElem)
{
    vector->ce = 0;
    vector->vec = malloc(CAP_INI * tamElem);

    if (vector->vec == NULL)
    {
        vector->cap = 0;
        return false;
    }

    vector->cap = CAP_INI;
    vector->tamElem = tamElem;

    return true;
}


bool vectorCrearDesdeArchivo(Vector* vector, size_t tamElem, const char* nomArch)
{
	FILE* arch = fopen(nomArch, "rb");

    if(!arch)
    {
        return false;
    }

	fseek(arch, 0L, SEEK_END);
	size_t tamArchBytes = ftell(arch);

	vector->vec = malloc(tamArchBytes);

	if(vector->vec == NULL)
    {
        vector->ce = 0;
		vector->cap = 0;
        return false;
    }

	size_t ce = tamArchBytes / tamElem;

	rewind(arch);
    fread(vector->vec, tamElem, ce, arch);

    fclose(arch);

	vector->cap = ce;
	vector->ce = ce;
	vector->tamElem = tamElem;

    return true;
}

int vectorOrdInsertar(Vector *vector, const void *elem, Cmp cmp)
{
    if (vector->ce == vector->cap)
    {
        if (!redimensionarVector(vector, FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    void *posIns = vector->vec;
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;

    while (posIns <= ult && cmp(elem, posIns) > 0)
    {
        posIns += vector->tamElem;
    }

    if (posIns <= ult && cmp(elem, posIns) == 0)
    {
        return DUPLICADO;
    }

    for (void *i = ult; i >= posIns; i -= vector->tamElem)
    {
        memcpy(i + vector->tamElem, i, vector->tamElem); // *(i + 1) = *i;
    }

    memcpy(posIns, elem, vector->tamElem);

    vector->ce++;

    return TODO_OK;
}

int vectorInsertarAlFinal(Vector *vector, const void *elem)
{
    if (vector->ce == vector->cap)
    {
        if (!redimensionarVector(vector, FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void *posIns = ult + vector->tamElem;
    memcpy(posIns, elem, vector->tamElem);
    vector->ce++;
    return TODO_OK;
}

int vectorOrdBuscar(const Vector *vector, void *elem, Cmp cmp)
{
    void *li = vector->vec;
    void *ls = vector->vec + (vector->ce - 1) * vector->tamElem;
    void *m;
    int comp;

    while (li < ls)
    {
        m = li + ((ls - li) / vector->tamElem / 2) * vector->tamElem;
        comp = cmp(elem, m);

        if (comp == 0)
        {
            memcpy(elem, m, vector->tamElem);
            return (m - vector->vec) / vector->tamElem;
        }

        if (comp < 0)
        {
            ls = m - vector->tamElem;
        }

        if (comp > 0)
        {
            li = m + vector->tamElem;
        }
    }

    return -1;
}

bool vectorOrdEliminarElem(Vector *vector, void *elem, Cmp cmp)
{
    if (vector->ce == 0)
    {
        return false;
    }

    void *posElim = vector->vec;
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;

    while (posElim <= ult && cmp(elem, posElim) > 0)
    {
        posElim += vector->tamElem;
    }

    if (posElim > ult || cmp(elem, posElim) < 0)
    {
        return false;
    }

    memcpy(elem, posElim, vector->tamElem);

    for (void *i = posElim; i < ult; i += vector->tamElem)
    {
        memcpy(i, i + vector->tamElem, vector->tamElem);
    }

    vector->ce--;

    if ((float)vector->ce / vector->cap <= FACTOR_OCUP)
    {
        redimensionarVector(vector, FACTOR_DECR);
    }

    return true;
}

int validarOrdenVector(Vector *vector, Cmp cmp)
{
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    for (void *i = vector->vec; i < ult; i += vector->tamElem)
    {
        if (cmp(i, i + vector->tamElem) > 0)
        {
            printf("ERROR el vector no esta ordenado\n");
            return 0;
        }
    }

    printf("Vector ordenado\n");
    return 1;
}

void vectorEliminar(Vector *vector)
{
    free(vector->vec);
    vector->vec = NULL;
    vector->ce = 0;
    vector->cap = 0;
}

void vectorVaciar(Vector *vector)
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

void vectorRecorrer(const Vector *vector, Accion accion, void *datosA)
{
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;

    for (void *i = vector->vec; i <= ult; i += vector->tamElem)
    {
        accion((i - vector->vec) / vector->tamElem, i, datosA);
    }
}

// Funciones privadas

bool redimensionarVector(Vector *vector, float factor)
{
    size_t nCap = vector->cap * factor;

    if (nCap < CAP_INI)
    {
        return false;
    }

    int *nVec = realloc(vector->vec, nCap * vector->tamElem);

    if (!nVec)
    {
        return false;
    }

    vector->vec = nVec;

    printf("Redimension de %llu a %llu\n", vector->cap, nCap);

    vector->cap = nCap;

    return true;
}

void vectorOrdenar(Vector *vector, int metodo, Cmp cmp)
{
    switch (metodo)
    {
    case BURBUJEO:
        ordenarBurbujeo(vector, cmp);
        break;

    case SELECCION:
        ordenarSeleccion(vector, cmp);
        break;

    case INSERCION:
        ordenarInsercion(vector, cmp);
        break;
    }
}

void ordenarBurbujeo(Vector *vector, Cmp cmp)
{
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    for (void *i = vector->vec + vector->tamElem, *limJ = ult - vector->tamElem; i < vector->vec + (vector->ce * vector->tamElem); i += vector->tamElem, limJ -= vector->tamElem)
    {
        for (void *j = vector->vec; j <= limJ; j += vector->tamElem)
        {
            if (cmp(j, j + vector->tamElem) > 0)
            {
                intercambiar(j, j + vector->tamElem, vector->tamElem);
            }
        }
    }
}

void ordenarSeleccion(Vector *vector, Cmp cmp)
{
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void *m = NULL;

    for (void *i = vector->vec; i < ult; i += vector->tamElem)
    {
        m = (void *)buscarMenor(i, ult, vector->tamElem, cmp);
        intercambiar(m, i, vector->tamElem);
    }
}

const void *buscarMenor(const void *ini, const void *fin, size_t tamElem, Cmp cmp)
{
    const void *m = ini;

    for (const void *j = ini + tamElem; j <= fin; j += tamElem)
    {
        if (cmp(j, m) < 0)
        {
            m = j;
        }
    }

    return m;
}

void ordenarInsercion(Vector *vector, Cmp cmp)
{
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void *elemAIns = malloc(vector->tamElem);

    for (void *i = vector->vec + vector->tamElem; i <= ult; i += vector->tamElem)
    {
        memcpy(elemAIns, i, vector->tamElem);

        void *j;
        for (j = i - vector->tamElem; j >= vector->vec && cmp(elemAIns, j) < 0; j -= vector->tamElem)
        {

            memcpy(j + vector->tamElem, j, vector->tamElem);
        }

        memcpy(j + vector->tamElem, elemAIns, vector->tamElem);
    }

    free(elemAIns);
}

void intercambiar(void *a, void *b, size_t tamElem)
{
    void *aTemp = malloc(tamElem);
    memcpy(aTemp, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aTemp, tamElem);
    free(aTemp);
}


int vectorGrabarEnArchivo(Vector* vector, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "wb");

    if(!arch)
    {
        return ERR_ARCHIVO;
    }

    fwrite(vector->vec, vector->tamElem, vector->ce, arch);

    fclose(arch);

    return TODO_OK;
}


// Iterador

void vectorIteradorCrear(VectorIterador* it, Vector* v)
{
    it->pri = v->vec;
    it->ult = v->vec + (v->ce - 1) * v->tamElem;
    it->act = NULL;
    it->finIter = true;
    it->tamElem = v->tamElem;
}


void* vectorIteradorPrimero(VectorIterador* it)
{
    if(it->ult < it->pri)
    {
        return NULL;
    }

    it->act = it->pri;
    it->finIter = false;

    return it->act;
}


void* vectorIteradorUltimo(VectorIterador* it)
{
    if(it->ult < it->pri)
    {
        return NULL;
    }

    it->act = it->ult;
    it->finIter = false;

    return it->act;
}


void* vectorIteradorSiguiente(VectorIterador* it)
{
    if(!it->act)
    {
        return vectorIteradorPrimero(it);
    }

    if(it->act == it->ult)
    {
        it->finIter = true;
        return NULL;
    }

    it->act += it->tamElem;

    return it->act;
}


void* vectorIteradorAnterior(VectorIterador* it)
{
    if(!it->act)
    {
        return vectorIteradorUltimo(it);
    }

    if(it->act == it->pri)
    {
        it->finIter = true;
        return NULL;
    }

    it->act -= it->tamElem;

    return it->act;
}


void* vectorIteradorActual(VectorIterador* it)
{
    return it->act;
}


bool vectorIteradorFin(VectorIterador* it)
{
    return it->finIter;
}
