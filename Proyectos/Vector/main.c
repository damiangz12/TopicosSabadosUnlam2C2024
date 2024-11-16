#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Vector.h"

#define CANT_ELEM 100000

int cmpInt(const void *e1, const void *e2);
void mostrarInt(int pos, void *e, void *datosA);
void cargarVectorRandom(Vector *vector, int ce);

int main()
{
	Vector miVec;
	vectorCrear(&miVec, sizeof(int));
	// int retIns;

	// for(int i = CANT_ELEM; i >= 1; i--)
	// {
	// 	retIns = vectorOrdInsertar(&miVec, &i, cmpInt);

	// 	if(retIns != TODO_OK)
	// 	{
	// 		return retIns;
	// 	}
	// }

	// vectorRecorrer(&miVec, mostrarInt, NULL);

	// for(int i = 1; i <= CANT_ELEM; i++)
	// {
	// 	vectorOrdEliminarElem(&miVec, i);
	// }

	cargarVectorRandom(&miVec, CANT_ELEM);

#if CANT_ELEM <= 100
	puts("Antes de ordenar:");
	vectorRecorrer(&miVec, mostrarInt, NULL);
#endif

	time_t iniT = time(NULL);
	vectorOrdenar(&miVec, INSERCION, cmpInt);
	time_t finT = time(NULL);

#if CANT_ELEM <= 100
	puts("\nDespues de ordenar:");
	vectorRecorrer(&miVec, mostrarInt, NULL);
#endif

	printf("\nTiempo de ejecucion: %ld\n", finT - iniT);

	validarOrdenVector(&miVec, cmpInt);

	vectorEliminar(&miVec);

	return 0;
}

int cmpInt(const void *e1, const void *e2)
{
	const int *i1 = e1;
	const int *i2 = e2;

	return *i1 - *i2;
}

void mostrarInt(int pos, void *e, void *datosA)
{
	int *i = e;
	printf("[%d]: %d\n", pos, *i);
}

void cargarVectorRandom(Vector *vector, int ce)
{
	int rnd;
	srand(time(NULL));

	for (int i = 0; i < ce; i++)
	{
		rnd = rand() % 100;
		vectorInsertarAlFinal(vector, &rnd);
	}
}
