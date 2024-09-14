#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#define TAM_VEC 10

#define TODO_OK 0
#define VEC_LLENO 1
#define DUPLICADO 2

typedef struct
{
    int vec[TAM_VEC];
    size_t ce;
}
Vector;

bool vectorCrear(Vector* vector);
int vectorOrdInsertar(Vector* vector, int elem);
int vectorOrdBuscar(const Vector* vector, int elem);
bool vectorOrdEliminarElem(Vector* vector, int elem);
void vectorEliminar(Vector* vec);
void vectorVaciar(Vector* vec);


#endif // VECTOR_H
