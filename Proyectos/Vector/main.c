#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Vector.h"


#define CANT_ELEM 100000


int main()
{
	Vector miVec;
	vectorCrear(&miVec);
	// int retIns;

	// for(int i = CANT_ELEM; i >= 1; i--)
	// {
	// 	retIns = vectorOrdInsertar(&miVec, i);

	// 	if(retIns != TODO_OK)
	// 	{
	// 		return retIns;
	// 	}
	// }

	// vectorMostrar(&miVec);

	// for(int i = 1; i <= CANT_ELEM; i++)
	// {
	// 	vectorOrdEliminarElem(&miVec, i);
	// }

	cargarVectorRandom(&miVec, CANT_ELEM);

#if CANT_ELEM <= 100
	puts("Antes de ordenar:");
	vectorMostrar(&miVec);
#endif

	time_t iniT = time(NULL);
	vectorOrdenar(&miVec, INSERCION);
	time_t finT = time(NULL);

#if CANT_ELEM <= 100
	puts("\nDespues de ordenar:");
	vectorMostrar(&miVec);
#endif

	printf("\nTiempo de ejecucion: %Id\n", finT - iniT);

	vectorEliminar(&miVec);

    return 0;
}
