#include <stdio.h>
#include <stdlib.h>
#include "Matriz.h"


#define FIL 4
#define COL 4


int main()
{
    // int matriz[FIL][COL] =
    // {
    //     {1, 2, 3, 4},
    //     {5, 6, 7, 8},
    //     {9, 10, 11, 12},
    //     {13, 14, 15, 16}
    // };
    int col = COL;
    // int (*matriz)[col] = malloc(sizeof(int) * FIL * col);

    int** matriz = (int**)crearMatriz(FIL, col, sizeof(int));

    if(!matriz)
    {
        puts("No se pudo reservar memoria.");
        return 1;
    }

    cargarMatriz(FIL, COL, matriz);

    mostrarMatriz(FIL, COL, matriz);

    int sumaDP = sumaDiagonalPrincipal(COL, matriz);
    printf("Suma DP: %d\n", sumaDP);

    int sumaDS = sumaDiagonalSecundaria(COL, matriz);
    printf("Suma DS: %d\n", sumaDS);

    int sumaTriInfDP = sumaTriangInfDP(COL, matriz);
    printf("Suma TriangInfDP: %d\n", sumaTriInfDP);
    
    int sumaTriSupDS = sumaTriangSupDS(COL, matriz);
    printf("Suma TriangSupDS: %d\n", sumaTriSupDS);

    destruirMatriz((void**)matriz, FIL);

    return 0;
}


