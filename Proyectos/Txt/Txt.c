#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Txt.h"


int archivoBinATxt(const char* nomArchBin, const char* nomArchTxt, size_t tamReg, BinATxt binATxt)
{
    FILE* archBin = fopen(nomArchBin, "rb");

    if(!archBin)
    {
        return ERR_ARCHIVO;
    }

    FILE* archTxt = fopen(nomArchTxt, "wt");

    if(!archTxt)
    {
        fclose(archBin);
        return ERR_ARCHIVO;
    }

    void* reg = malloc(tamReg);

    if(!reg)
    {
        fclose(archBin);
        fclose(archTxt);
        return ERR_SIN_MEM;
    }


    while(fread(reg, tamReg, 1, archBin))
    {
        binATxt(reg, archTxt);
    }

    free(reg);
    fclose(archBin);
    fclose(archTxt);

    return TODO_OK;
}


int archivoTxtABin(const char* nomArchTxt, const char* nomArchBin, size_t tamReg, TxtABin txtABin, ErrorFatal errorFatal)
{
    FILE* archBin = fopen(nomArchBin, "wb");

    if(!archBin)
    {
        return ERR_ARCHIVO;
    }

    FILE* archTxt = fopen(nomArchTxt, "rt");

    if(!archTxt)
    {
        fclose(archBin);
        return ERR_ARCHIVO;
    }

    void* reg = malloc(tamReg);

    if(!reg)
    {
        fclose(archBin);
        fclose(archTxt);
        return ERR_SIN_MEM;
    }

    int ret = TODO_OK;
    char linea[TAM_LINEA];

    while(fgets(linea, TAM_LINEA, archTxt) && !errorFatal(ret))
    {
        ret = txtABin(linea, reg);
        
        if(ret == TODO_OK)
        {
            fwrite(reg, tamReg, 1, archBin);
        }
    }
    
    free(reg);
    fclose(archBin);
    fclose(archTxt);

    return ret;
}


bool esConversionBinATxt(const char* nomArch)
{
    char* ext = strchr(nomArch, '.') + 1;
    return strcmp(ext, "dat") == 0;
}
