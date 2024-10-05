#ifndef SECPAL_H
#define SECPAL_H

#include <stdbool.h>
#include "Palabra.h"


typedef struct
{
    char* cursor;
    bool finSec;
}
SecPal;


void secPalCrear(SecPal* sec, const char* cad);
bool secPalLeer(SecPal* sec, Palabra* pal);
void secPalEscribir(SecPal* sec, const Palabra* pal);
void secPalEscribirCar(SecPal* sec, char c);
bool secPalFin(const SecPal* sec);
void secPalCerrar(SecPal* sec);


#endif // SECPAL_H
