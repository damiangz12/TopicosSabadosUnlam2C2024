#ifndef TXT_H
#define TXT_H

#include <stdbool.h>

#define TODO_OK 0
#define ERR_ARCHIVO 1
#define ERR_SIN_MEM 2

#define TAM_LINEA 501


typedef void (*BinATxt)(const void* reg, FILE* archTxt);
typedef int (*TxtABin)(char* linea, void* reg);
typedef bool (*ErrorFatal)(int cod);


bool esConversionBinATxt(const char* nomArch);
int archivoBinATxt(const char* nomArchBin, const char* nomArchTxt, size_t tamReg, BinATxt binATxt);
int archivoTxtABin(const char* nomArchTxt, const char* nomArchBin, size_t tamReg, TxtABin txtABin, ErrorFatal errorFatal);


#endif // TXT_H
