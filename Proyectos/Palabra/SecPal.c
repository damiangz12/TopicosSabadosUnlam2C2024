#include "SecPal.h"


void secPalCrear(SecPal* sec, const char* cad)
{
    sec->cursor = cad;
    sec->finSec = false;
}


bool secPalLeer(SecPal* sec, Palabra* pal)
{
    while(*(sec->cursor) && !esLetra(*(sec->cursor)))
    {
        sec->cursor++;
    }

    if(!*(sec->cursor))
    {
        sec->finSec = true;
        return false;
    }

    char* actPal = pal->vPal;
    while(*(sec->cursor) && esLetra(*(sec->cursor)))
    {
        *actPal = *(sec->cursor);
        actPal++;
        sec->cursor++;
    }

    *actPal = '\0';
    pal->longitud = actPal - pal->vPal;

    return true;
}


void secPalEscribir(SecPal* sec, const Palabra* pal)
{
    char* actPal = pal->vPal;
    while(*actPal)
    {
        *(sec->cursor) = *actPal;
        sec->cursor++;
        actPal++;
    }
}


void secPalEscribirCar(SecPal* sec, char c)
{
    *(sec->cursor) = c;
    sec->cursor++;
}


bool secPalFin(const SecPal* sec)
{
    return sec->finSec;
}


void secPalCerrar(SecPal* sec)
{
    *(sec->cursor) = '\0';
}
