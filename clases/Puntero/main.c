#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int dia;
    int mes;
    int anio;
}
Fecha;

void intercambiar(int* x, int* y);
void mostrarFecha(const Fecha* f);


int main()
{
    int a = 5;
    int b = 8;

    intercambiar(&a, &b);

    printf("a: %d, b: %d\n", a, b);

    Fecha fecha = {31, 8, 2024};

    mostrarFecha(&fecha);

    return 0;
}

void intercambiar(int* x, int* y)
{
    int xTemp =*x;
    *x = *y;
    *y = xTemp;
}


void mostrarFecha(const Fecha* f)
{
    printf("%d/%d/%d", f->dia, f->mes, f->anio);
}