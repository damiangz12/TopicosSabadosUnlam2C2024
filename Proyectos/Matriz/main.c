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
    int fil = FIL;
    // int (*matriz)[col] = malloc(sizeof(int) * FIL * col);

    int** matriz = (int**)crearMatriz(fil, col, sizeof(int));

    if(!matriz)
    {
        puts("No se pudo reservar memoria.");
        return 1;
    }

    cargarMatriz(fil, col, matriz);

    mostrarMatriz(fil, col, matriz);

    int sumaDP = sumaDiagonalPrincipal(col, matriz);
    printf("Suma DP: %d\n", sumaDP);

    int sumaDS = sumaDiagonalSecundaria(col, matriz);
    printf("Suma DS: %d\n", sumaDS);

    int sumaTriInfDP = sumaTriangInfDP(col, matriz);
    printf("Suma TriangInfDP: %d\n", sumaTriInfDP);
    
    int sumaTriSupDS = sumaTriangSupDS(col, matriz);
    printf("Suma TriangSupDS: %d\n", sumaTriSupDS);

    destruirMatriz((void**)matriz, fil);

    return 0;
}


