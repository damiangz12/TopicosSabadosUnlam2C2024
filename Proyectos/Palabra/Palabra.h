#ifndef PALABRA_H
#define PALABRA_H

#include <stdlib.h>

#define TAM_PAL 51

#define esLetra(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))

#define aMayuscula(c) ((c) >= 'a' && (c) <= 'z' ? (c) - ('a' - 'A') : (c))

#define aMinuscula(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + ('a' - 'A') : (c))


typedef struct
{
    char vPal[TAM_PAL];
    size_t longitud;
}
Palabra;


void palabraATitulo(Palabra* pal);


#endif // PALABRA_H
