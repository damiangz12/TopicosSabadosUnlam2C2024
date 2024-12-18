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
#define ERR_ARCHIVO 3

#define BURBUJEO 1
#define SELECCION 2
#define INSERCION 3


typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
Vector;


typedef struct
{
    void* pri;
    void* ult;
    void* act;
    bool finIter;
    size_t tamElem;
}
VectorIterador;


typedef int (*Cmp)(const void* e1, const void* e2);
typedef void (*Accion)(int pos, void* e, void* datosA);


bool vectorCrear(Vector* vector, size_t tamElem); // malloc
bool vectorCrearDesdeArchivo(Vector* vector, size_t tamElem, const char* nomArch);
int vectorOrdInsertar(Vector* vector, const void* elem, Cmp cmp); // realloc
int vectorInsertarAlFinal(Vector* vector, const void* elem);
int vectorOrdBuscar(const Vector* vector, void* elem, Cmp cmp);
bool vectorOrdEliminarElem(Vector* vector, void* elem, Cmp cmp); // realloc
void vectorEliminar(Vector* vector); // free
void vectorVaciar(Vector* vector); // realloc
void vectorMostrar(const Vector* vector);
void vectorRecorrer(const Vector* vector, Accion accion, void* datosA);
void vectorOrdenar(Vector* vector, int metodo, Cmp cmp);
int validarOrdenVector(Vector *vector, Cmp cmp);
int vectorGrabarEnArchivo(Vector* vector, const char* nomArch);

// Iterador

void vectorIteradorCrear(VectorIterador* it, Vector* v);
void* vectorIteradorPrimero(VectorIterador* it);
void* vectorIteradorUltimo(VectorIterador* it);
void* vectorIteradorSiguiente(VectorIterador* it);
void* vectorIteradorAnterior(VectorIterador* it);
void* vectorIteradorActual(VectorIterador* it);
bool vectorIteradorFin(VectorIterador* it);

#endif // VECTOR_H
