#include "Palabra.h"


void palabraATitulo(Palabra* pal)
{
    char* actPal = pal->vPal;
    *actPal = aMayuscula(*actPal);
    actPal++;
    
    while(*actPal)
    {
        *actPal = aMinuscula(*actPal);
        actPal++;
    }
}