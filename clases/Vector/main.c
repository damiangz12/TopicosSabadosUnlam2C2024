#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

#define CANT_ELEM 20

int main()
{
    Vector miVec;
    vectorCrear(&miVec);
    int retIns;

    for(int i= CANT_ELEM; i >= 1; i--)
    {
        retIns = vectorOrdInsertar(&miVec, i);

        if(retIns != TODO_OK)
        {
            return retIns;
        }
    }

    vectorMostrar(&miVec);

    for(int i = 1; i <= CANT_ELEM; i++)
    {
        vectorOrdEliminarElem(&miVec, i);
    }

    vectorEliminar(&miVec);

    return 0;
}
