#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Indice.h"


int generarIndice(const char* nomArchivo, size_t tamReg, size_t tamRegIdx, CrearRegIdx crearRegIdx, Cmp cmpRegIdx)
{
    FILE* arch = fopen(nomArchivo, "rb");

    if(!arch)
    {
        return ERR_ARCHIVO;
    }

    void* reg = malloc(tamReg);
    void* regIdx = malloc(tamRegIdx);

    if(!reg || !regIdx)
    {
        free(reg);
        free(regIdx);
        fclose(arch);
        return ERR_SIN_MEMORIA;
    }

    Vector vecIdx;

    if(!vectorCrear(&vecIdx, tamRegIdx))
    {
        free(reg);
        free(regIdx);
        fclose(arch);
        return ERR_SIN_MEMORIA;
    }

    int nroReg = 0;

    while(fread(reg, tamReg, 1, arch))
    {
        crearRegIdx(reg, nroReg++, regIdx);
        vectorOrdInsertar(&vecIdx, regIdx, cmpRegIdx);
    }

    free(reg);
    free(regIdx);
    fclose(arch);

    char nomArchivoIdx[51];
    strcpy(nomArchivoIdx, nomArchivo);
    char* extension = strchr(nomArchivoIdx, '.') + 1;
    strcpy(extension, "idx");

    vectorGrabarEnArchivo(&vecIdx, nomArchivoIdx);

    vectorEliminar(&vecIdx);

    return TODO_OK;
}
