#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Indice.h"


#define TAM_CODIGO 4
#define TAM_DESCRIPCION 20

#define CANT_PRODS 10

#define TODO_OK 0
#define ERR_NO_ENCONTRADO 4

#define ARG_NOM_ARCH_PROD 1
#define ARG_PORC_INCR 2


typedef struct
{
    char codigo[TAM_CODIGO + 1];
    char descripcion[TAM_DESCRIPCION + 1];
    float precio;
    int stock;
}
Producto;


typedef struct
{
    char codigo[TAM_CODIGO + 1];
    int nroReg;
}
ProductoIdx;


int generarProductos(const char* nomArchProductos);
int mostrarProductos(const char* nomArchProductos);
void mostrarIndice(const char* nomArchProductos);
int actualizarPrecioProductos(const char* nomArchProductos, float porc);
int actualizarPrecio1Producto(const char* nomArchProductos, const char* codProd, float porc);
void crearProductoIdx(const void* regArch, int nroReg, void* regIdx);
int cmpProductoIdx(const void* e1, const void* e2);


// Archivos.exe Productos.dat 10.5

int main(int argc, char* argv[])
{
    generarProductos(argv[ARG_NOM_ARCH_PROD]);
    generarIndice(argv[ARG_NOM_ARCH_PROD], sizeof(Producto), sizeof(ProductoIdx), crearProductoIdx, cmpProductoIdx);
    
    puts("Indice:");
    mostrarIndice(argv[ARG_NOM_ARCH_PROD]);

    puts("Antes de actualizar:");
    mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

    float porcIncr = atof(argv[ARG_PORC_INCR]);

    actualizarPrecioProductos(argv[ARG_NOM_ARCH_PROD], porcIncr); // sscanf(argv[ARG_PORC_INCR], "%f", &porcF);

    puts("Despues de actualizar:");
    mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

    actualizarPrecio1Producto(argv[ARG_NOM_ARCH_PROD], "CERE", porcIncr);

    puts("Despues de actualizar CERE:");
    mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

    return 0;
}


int generarProductos(const char* nomArchProductos)
{
    Producto productos[CANT_PRODS] =
    {
        {"PERA", "Peras", 100.0, 100},
        {"BANA", "Banana Ecuador", 200.0, 230},
        {"MANZ", "Manzana Roja", 300.0, 430},
        {"NARA", "Naranja", 450.0, 240},
        {"DURA", "Durazno", 500.0, 650},
        {"UVAS", "Uvas", 600.0, 160},
        {"CERE", "Cerezas", 700.0, 770},
        {"FRUT", "Frutillas", 800.0, 380},
        {"KIWI", "Kiwi", 900.0, 690},
        {"MELO", "Melon", 1000.0, 1100}
    };

    FILE* archProd = fopen(nomArchProductos, "wb");

    if(!archProd)
    {
        return ERR_ARCHIVO;
    }

    fwrite(productos, sizeof(Producto), CANT_PRODS, archProd);

    fclose(archProd);

    return TODO_OK;
}


int mostrarProductos(const char* nomArchProductos)
{
    FILE* archProd = fopen(nomArchProductos, "rb");

    if(!archProd)
    {
        return ERR_ARCHIVO;
    }

    // fseek(archProd, 240, SEEK_SET);

    Producto prod;

    while(fread(&prod, sizeof(Producto), 1, archProd))
    {
        printf("%-4s, %-20s, %8.2f, %4d\n", prod.codigo, prod.descripcion, prod.precio, prod.stock);
    }

    fclose(archProd);

    return TODO_OK;
}


/***************************************************************
r+b --> Lect y escr. El arch debe existir. No pisa cont previo.
w+b --> Lect y escr. Si no existe lo crea. Pisa el cont previo.
***************************************************************/
int actualizarPrecioProductos(const char* nomArchProductos, float porc)
{
    FILE* archProd = fopen(nomArchProductos, "r+b");

    if(!archProd)
    {
        return ERR_ARCHIVO;
    }

    float factorIncr = 1 + porc / 100;
    Producto prod;

    while(fread(&prod, sizeof(Producto), 1, archProd))
    {
        prod.precio *= factorIncr;
        fseek(archProd, -(long)sizeof(Producto), SEEK_CUR);
        fwrite(&prod, sizeof(Producto), 1, archProd);
        fflush(archProd); // Necesario cada vez que cambiamos el sentido (r->w, o w->r).
    }

    fclose(archProd);

    return TODO_OK;
}


int actualizarPrecio1Producto(const char* nomArchProductos, const char* codProd, float porc)
{
    FILE* archProd = fopen(nomArchProductos, "r+b");

    if(!archProd)
    {
        return ERR_ARCHIVO;
    }

    char nomArchivoIdx[51];
    strcpy(nomArchivoIdx, nomArchProductos);
    char* extension = strchr(nomArchivoIdx, '.') + 1;
    strcpy(extension, "idx");

    Vector vecProdIdx;
    if(!vectorCrearDesdeArchivo(&vecProdIdx, sizeof(ProductoIdx), nomArchivoIdx))
    {
        fclose(archProd);
        return ERR_SIN_MEMORIA;
    }

    ProductoIdx prodIdx;
    strcpy(prodIdx.codigo, codProd);

    if(vectorOrdBuscar(&vecProdIdx, &prodIdx, cmpProductoIdx) == -1)
    {
        fclose(archProd);
        vectorEliminar(&vecProdIdx);
        return ERR_NO_ENCONTRADO;
    }

    Producto prod;

    fseek(archProd, prodIdx.nroReg * sizeof(Producto), SEEK_SET);
    fread(&prod, sizeof(Producto), 1, archProd);

    float factorIncr = 1 + porc / 100;
    prod.precio *= factorIncr;
    fseek(archProd, prodIdx.nroReg * sizeof(Producto), SEEK_SET);
    fwrite(&prod, sizeof(Producto), 1, archProd);

    fclose(archProd);
    vectorEliminar(&vecProdIdx);

    return TODO_OK;
}


void crearProductoIdx(const void* regArch, int nroReg, void* regIdx)
{
    const Producto* prod = regArch;
    ProductoIdx* prodIdx = regIdx;

    strcpy(prodIdx->codigo, prod->codigo);
    prodIdx->nroReg = nroReg;
}


int cmpProductoIdx(const void* e1, const void* e2)
{
    const ProductoIdx* prodIdx1 = e1;
    const ProductoIdx* prodIdx2 = e2;

    return strcmp(prodIdx1->codigo, prodIdx2->codigo);
}


void mostrarIndice(const char* nomArchProductos)
{
    char nomArchivoIdx[51];
    strcpy(nomArchivoIdx, nomArchProductos);
    char* extension = strchr(nomArchivoIdx, '.') + 1;
    strcpy(extension, "idx");

    FILE* archIdx = fopen(nomArchivoIdx, "rb");

    if(!archIdx)
    {
        return;
    }

    ProductoIdx prodIdx;
    while(fread(&prodIdx, sizeof(ProductoIdx), 1, archIdx))
    {
        printf("%-4s, %d\n", prodIdx.codigo, prodIdx.nroReg);
    }

    fclose(archIdx);
}