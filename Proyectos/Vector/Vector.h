#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

#define CAP_INI 10
#define FACTOR_INCR 1.5
#define FACTOR_OCUP 0.25
#define FACTOR_DECR 0.5

#define TODO_OK 0
#define VEC_LLENO 1
#define DUPLICADO 2

#define BURBUJEO 1
#define SELECCION 2
#define INSERCION 3


typedef struct
{
    int* vec;
    size_t ce;
    size_t cap;
}
Vector;


bool vectorCrear(Vector* vector); // malloc
int vectorOrdInsertar(Vector* vector, int elem); // realloc
int vectorInsertarAlFinal(Vector* vector, int elem);
int vectorOrdBuscar(const Vector* vector, int elem);
bool vectorOrdEliminarElem(Vector* vector, int elem); // realloc
void vectorEliminar(Vector* vector); // free
void vectorVaciar(Vector* vector); // realloc
void vectorMostrar(const Vector* vector);
void vectorOrdenar(Vector* vector, int metodo);
void cargarVectorRandom(Vector* vector, int ce);


#endif // VECTOR_H
