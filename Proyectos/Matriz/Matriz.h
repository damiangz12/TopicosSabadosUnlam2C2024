#ifndef MATRIZ_H
#define MATRIZ_H

#include <stddef.h>


void** crearMatriz(int fil, int col, size_t tamElem);
void destruirMatriz(void** mat, int fil);
void cargarMatriz(int fil, int col, int** mat);
void mostrarMatriz(int fil, int col, int** mat);
int sumaDiagonalPrincipal(int orden, int** mat);
int sumaDiagonalSecundaria(int orden, int** mat);
int sumaTriangInfDP(int orden, int** mat);
int sumaTriangSupDS(int orden, int** mat);


#endif // MATRIZ_H
