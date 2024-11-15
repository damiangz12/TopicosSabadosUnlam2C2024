#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Txt.h"

#define TAM_DNI 8
#define TAM_APYN 30
#define TAM_SEXO 1
#define TAM_FINGR 8
#define TAM_SUELDO 10

#define ERR_LINEA_LARGA 1


typedef struct
{
    int dia;
    int mes;
    int anio;
}
Fecha;


typedef struct
{
    int dni;
    char apyn[TAM_APYN + 1];
    char sexo;
    Fecha fIngr;
    float sueldo;
}
Empleado;


int generarEmpleadosBin(const char* nomArchEmpleados);
void empleadoBinATxtV(const void* reg, FILE* archTxt);
void empleadoBinATxtF(const void* reg, FILE* archTxt);
int empleadoTxtVABin(char* linea, void* reg);
int empleadoTxtFABin(char* linea, void* reg);
bool empleadoErrorFatal(int cod);
int mostrarEmpleados(const char* nomArchEmpleados);


// Txt Empleados.dat EmpleadosV.txt V --> Bin a Txt var
// Txt Empleados.dat EmpleadosF.txt F --> Bin a Txt Fijo
// Txt Empleados.txt V Empleados.dat --> Txt var a Bin

int main(int argc, char* argv[])
{
    //TODO: Validar argumentos.
    
    // generarEmpleadosBin("Empleados.dat");
    
    char formatoTxt;

    if(esConversionBinATxt(argv[1]))
    {
        formatoTxt = argv[3][0];
        archivoBinATxt(argv[1], argv[2], sizeof(Empleado), formatoTxt == 'V'? empleadoBinATxtV : empleadoBinATxtF);
    }
    else
    {
        formatoTxt = argv[2][0];
        archivoTxtABin(argv[1], argv[3], sizeof(Empleado), formatoTxt == 'V'? empleadoTxtVABin : empleadoTxtFABin, empleadoErrorFatal);
        mostrarEmpleados(argv[3]);
    }

    return TODO_OK;
}


int generarEmpleadosBin(const char* nomArchEmpleados)
{
    FILE* archEmpleados = fopen(nomArchEmpleados, "wb");

    if(archEmpleados == NULL)
    {
        return ERR_ARCHIVO;
    }

    Empleado empleados[] = {
        {12345678, "Juan Perez", 'M', {1, 1, 2000}, 100000.0},
        {23456789, "Ana Lopez", 'F', {2, 2, 2001}, 200000.0},
        {34567890, "Pedro Gomez", 'M', {3, 3, 2002}, 300000.0},
        {45678901, "Maria Rodriguez", 'F', {4, 4, 2003}, 400000.0},
        {5678901, "Carlos Sanchez", 'M', {5, 5, 2004}, 500000.0},
        {67890123, "Laura Fernandez", 'F', {6, 6, 2005}, 600000.0},
        {78901234, "Jorge Gonzalez", 'M', {7, 7, 2006}, 700000.0},
        {89012345, "Silvia Ramirez", 'F', {8, 8, 2007}, 800000.0},
        {90123456, "Miguel Torres", 'M', {9, 9, 2008}, 900000.0},
        {11234567, "Cecilia Castro", 'F', {10, 10, 2009}, 1000000.0}
    };

    printf("Tam array empleados: %Iu\n", sizeof(empleados));
    printf("Tam Empleado: %Iu\n", sizeof(Empleado));
    printf("Cant reg: %Iu\n", sizeof(empleados) / sizeof(Empleado));

    fwrite(empleados, sizeof(Empleado), sizeof(empleados) / sizeof(Empleado), archEmpleados);

    fclose(archEmpleados);

    return TODO_OK;
}


void empleadoBinATxtV(const void* reg, FILE* archTxt)
{
    const Empleado* empl = reg;
    fprintf(archTxt, "%d|%s|%c|%d/%d/%d|%.2f\n", empl->dni, empl->apyn, empl->sexo, empl->fIngr.dia, empl->fIngr.mes, empl->fIngr.anio, empl->sueldo);
}


void empleadoBinATxtF(const void* reg, FILE* archTxt)
{
    const Empleado* empl = reg;
    fprintf(archTxt, "%0*d%-*s%*c%02d%02d%04d%0*.2f\n", TAM_DNI, empl->dni, TAM_APYN, empl->apyn, TAM_SEXO, empl->sexo, empl->fIngr.dia, empl->fIngr.mes, empl->fIngr.anio, TAM_SUELDO, empl->sueldo);
}


int empleadoTxtVABin(char* linea, void* reg)
{
    char* act = strchr(linea, '\n');

    if(!act)
    {
        return ERR_LINEA_LARGA;
    }

    Empleado* empl = reg;

    // Sueldo
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act + 1, "%f", &empl->sueldo);

    // Fecha
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act + 1, "%d/%d/%d", &empl->fIngr.dia, &empl->fIngr.mes, &empl->fIngr.anio);

    // Sexo
    *act = '\0';
    act = strrchr(linea, '|');
    empl->sexo = *(act + 1);

    // ApYN
    *act = '\0';
    act = strrchr(linea, '|');
    strncpy(empl->apyn, act + 1, TAM_APYN);
    empl->apyn[TAM_APYN] = '\0';

    // DNI
    *act = '\0';
    sscanf(linea, "%d", &empl->dni);

    return TODO_OK;
}


int empleadoTxtFABin(char* linea, void* reg)
{
    char* act = strchr(linea, '\n');

    if(!act)
    {
        return ERR_LINEA_LARGA;
    }

    Empleado* empl = reg;

    // Sueldo
    *act = '\0';
    act -= TAM_SUELDO;
    sscanf(act, "%f", &empl->sueldo);

    // Fecha
    *act = '\0';
    act -= TAM_FINGR;
    sscanf(act, "%2d%2d%4d", &empl->fIngr.dia, &empl->fIngr.mes, &empl->fIngr.anio);

    // Sexo
    *act = '\0';
    act -= TAM_SEXO;
    empl->sexo = *act;

    // ApYN
    *act = '\0';
    act -= TAM_APYN;
    strncpy(empl->apyn, act, TAM_APYN);
    empl->apyn[TAM_APYN] = '\0';

    // DNI
    *act = '\0';
    sscanf(linea, "%d", &empl->dni);

    return TODO_OK;
}


bool empleadoErrorFatal(int cod)
{
    return cod == ERR_LINEA_LARGA;
}


int mostrarEmpleados(const char* nomArchEmpleados)
{
    FILE* archEmpleados = fopen(nomArchEmpleados, "rb");

    if(archEmpleados == NULL)
    {
        return ERR_ARCHIVO;
    }

    Empleado empl;

    while(fread(&empl, sizeof(Empleado), 1, archEmpleados))
    {
        printf("%d\t%s\t%c\t%d/%d/%d\t%.2f\n", empl.dni, empl.apyn, empl.sexo, empl.fIngr.dia, empl.fIngr.mes, empl.fIngr.anio, empl.sueldo);
    }

    fclose(archEmpleados);

    return TODO_OK;
}