#ifndef INDICE_H
#define INDICE_H

#include "../Vector/Vector.h"

#define ERR_ARCHIVO 3
#define ERR_SIN_MEMORIA 2


typedef void (*CrearRegIdx)(const void* regArch, int nroReg, void* regIdx);


int generarIndice(const char* nomArchivo, size_t tamReg, size_t tamRegIdx, CrearRegIdx crearRegIdx, Cmp cmpRegIdx);


#endif // INDICE_H
