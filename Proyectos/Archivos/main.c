#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Indice.h"


#define TAM_CODIGO 4
#define TAM_DESCRIPCION 20

#define CANT_PRODS 10
#define CANT_MOVS 13

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


typedef struct
{
    char codigo[TAM_CODIGO + 1];
    int cantidad;
}
Movimiento;


int generarProductos(const char* nomArchProductos);
int generarMovimientos(const char* nomArchMovimientos);
int mostrarProductos(const char* nomArchProductos);
void mostrarIndice(const char* nomArchProductos);
int actualizarPrecioProductos(const char* nomArchProductos, float porc);
int actualizarPrecio1Producto(const char* nomArchProductos, const char* codProd, float porc);
void crearProductoIdx(const void* regArch, int nroReg, void* regIdx);
int cmpProductoIdx(const void* e1, const void* e2);
int actualizarStockProductos(const char* nomArchProductos, const char* nomArchMovimientos); // Merge
bool leerProducto(Producto* prod, FILE* archProd, VectorIterador* itIdx);
void escribirProductoNuevo(Producto* prod, FILE* archProd);


// Archivos.exe Productos.dat 10.5

int main(int argc, char* argv[])
{
    generarProductos(argv[ARG_NOM_ARCH_PROD]);
    generarMovimientos("Movimientos.dat");

    generarIndice(argv[ARG_NOM_ARCH_PROD], sizeof(Producto), sizeof(ProductoIdx), crearProductoIdx, cmpProductoIdx);

    puts("Indice:");
    mostrarIndice(argv[ARG_NOM_ARCH_PROD]);

    puts("Antes de actualizar:");
    mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

//    float porcIncr = atof(argv[ARG_PORC_INCR]);

    // actualizarPrecioProductos(argv[ARG_NOM_ARCH_PROD], porcIncr); // sscanf(argv[ARG_PORC_INCR], "%f", &porcF);

    // puts("Despues de actualizar:");
    // mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

    // actualizarPrecio1Producto(argv[ARG_NOM_ARCH_PROD], "CERE", porcIncr);

    // puts("Despues de actualizar CERE:");
    // mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

    actualizarStockProductos(argv[ARG_NOM_ARCH_PROD], "Movimientos.dat");

    puts("Despues de actualizar Stocks:");
    mostrarProductos(argv[ARG_NOM_ARCH_PROD]);

    puts("Indice:");
    mostrarIndice(argv[ARG_NOM_ARCH_PROD]);

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


int generarMovimientos(const char* nomArchMovimientos)
{
    // Generar movimientos ordenados por codigo de producto.

    Movimiento movimientos[] =
    {
        {"BANA", 10},
        {"BANA", 20},
        {"BANA", -5},
        {"CERE", 30},
        {"CERE", -10},
        {"CERE", 20},
        {"DURA", 15},
        {"DURA", -5},
        {"DURA", 10},
        {"FRUT", 25},
        {"MAND", 40},
        {"MAND", -10},
        {"SAND", 30}
    };

    FILE* archMov = fopen(nomArchMovimientos, "wb");

    if(!archMov)
    {
        return ERR_ARCHIVO;
    }

    fwrite(movimientos, sizeof(Movimiento), CANT_MOVS, archMov);

    fclose(archMov);

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


int actualizarStockProductos(const char* nomArchProductos, const char* nomArchMovimientos) // Merge
{
    FILE* archProd = fopen(nomArchProductos, "r+b");

    if(!archProd)
    {
        return ERR_ARCHIVO;
    }

    FILE* archMov = fopen(nomArchMovimientos, "rb");

    if(!archMov)
    {
        return ERR_ARCHIVO;
    }

    char nomArchProdIdx[51];
    strcpy(nomArchProdIdx, nomArchProductos);
    char* ext = strchr(nomArchProdIdx, '.') + 1;
    strcpy(ext, "idx");

    Vector vIndice;
    vectorCrearDesdeArchivo(&vIndice, sizeof(ProductoIdx), nomArchProdIdx);
    VectorIterador itIdx;
    vectorIteradorCrear(&itIdx, &vIndice);

    Producto prod, prodNuevo;
    Movimiento mov;
    bool hayProducto;
    int comp;

    hayProducto = leerProducto(&prod, archProd, &itIdx);
    fread(&mov, sizeof(Movimiento), 1, archMov);
    while(hayProducto && !feof(archMov))
    {
        comp = strcmp(prod.codigo, mov.codigo);

        if(comp == 0)
        {
            prod.stock += mov.cantidad;
            fread(&mov, sizeof(Movimiento), 1, archMov);
        }

        if(comp < 0) // Prod sin (mas) movs
        {
            fseek(archProd, -sizeof(Producto), SEEK_CUR);
            fwrite(&prod, sizeof(Producto), 1, archProd);
            hayProducto = leerProducto(&prod, archProd, &itIdx);
        }

        if(comp > 0) // Prod nuevo
        {
            strcpy(prodNuevo.codigo, mov.codigo);
            prodNuevo.stock = mov.cantidad;
            prodNuevo.descripcion[0] = '\0';
            prodNuevo.precio = 0;
            fread(&mov, sizeof(Movimiento), 1, archMov);
            while(!feof(archMov) && strcmp(prodNuevo.codigo, mov.codigo) == 0)
            {
                prodNuevo.stock += mov.cantidad;
                fread(&mov, sizeof(Movimiento), 1, archMov);
            }

            escribirProductoNuevo(&prodNuevo, archProd);
        }
    }

    // while(hayProducto) // Prod sin movs. Aplica sólo para cuando generamos un archivo nuevo.
    // {
    //     escribirProductoNuevo(&prod, archProd);
    //     hayProducto = leerProducto(&prod, archProd);
    // }

    while(!feof(archMov)) // Prod nuevo
    {
        strcpy(prodNuevo.codigo, mov.codigo);
        prodNuevo.stock = mov.cantidad;
        prodNuevo.descripcion[0] = '\0';
        prodNuevo.precio = 0;
        fread(&mov, sizeof(Movimiento), 1, archMov);
        while(!feof(archMov) && strcmp(prodNuevo.codigo, mov.codigo) == 0)
        {
            prodNuevo.stock += mov.cantidad;
            fread(&mov, sizeof(Movimiento), 1, archMov);
        }

        escribirProductoNuevo(&prodNuevo, archProd);
    }

    fclose(archProd);
    fclose(archMov);
    vectorEliminar(&vIndice);

    generarIndice(nomArchProductos, sizeof(Producto), sizeof(ProductoIdx), crearProductoIdx, cmpProductoIdx);

    return TODO_OK;
}


bool leerProducto(Producto* prod, FILE* archProd, VectorIterador* itIdx)
{
    ProductoIdx* idx = vectorIteradorSiguiente(itIdx);

    if(vectorIteradorFin(itIdx))
    {
        return false;
    }

    fseek(archProd, idx->nroReg * sizeof(Producto), SEEK_SET);
    fread(prod, sizeof(Producto), 1, archProd);

    return true;
}


void escribirProductoNuevo(Producto* prod, FILE* archProd)
{
    fseek(archProd, 0L, SEEK_END);
    fwrite(prod, sizeof(Producto), 1, archProd);
}
