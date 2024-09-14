#include <stdbool.h>
#include <stdio.h>

typedef struct
{
        int dia;
        int mes;
        int anio;
}
Fecha;

Fecha fechaCrear (int d, int m, int a){

    Fecha fecha = {d,m,a};
    return fecha;

}

bool esBisiesto(int a){
    return a % 4 == 0 && (a % 100 != 0 || a % 400 == 0);
}

int cantDiasMes(int mes, int anio){
 int vcdm[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

 if(mes==2&&esBisiesto(anio))
    return 29;

 return vcdm[mes];
}

bool esFechaValida(int d, int m, int a){
    return a>=1601&&m>=1&&m<=12&&d>=1&&d<=cantDiasMes(m, a);
}

Fecha ingresarFecha()
{
    int d,m,a;

    puts("Ingrese una fecha (d/m/a): ");
    scanf("%d/%d/%d", &d, &m, &a);

    while(!esFechaValida(d,m,a)){
            puts("Fecha Invalida. Ingrese de nuevo (d/m/a): ");
            scanf("%d/%d/%d", &d, &m, &a);

    }

    return fechaCrear(d,m,a);
}

int fechaADias(Fecha fecha){

    return 0;

}

Fecha diasAFecha(int dias){

    Fecha fecha = {0,0,0};

    return fecha;

}

Fecha fechaSumarDias(Fecha fecha, int dias){
    int cdm;
    Fecha suma = fecha;
    suma.dia+=dias;

    while(suma.dia>(cdm=cantDiasMes(suma.mes, suma.anio))){
        suma.dia-=cdm;
        suma.mes++;
        if(suma.mes>12){
            suma.mes=1;
            suma.anio++;
        }
    }

    return suma;
}

int ingresarEnteroPositivo(){
    int d;

    puts("Ingrese una cantidad de dias: ");
    scanf("%d", &d);

    return d;

}

void mostrarFecha(Fecha fecha){

    printf("La fecha es: %d/%d/%d", fecha.dia, fecha.mes, fecha.anio);
    return;

}


void main()
{
    Fecha fecha = ingresarFecha();
    int dias = ingresarEnteroPositivo();
    Fecha fSuma = fechaSumarDias(fecha, dias);
    mostrarFecha(fSuma);
    return;
}
