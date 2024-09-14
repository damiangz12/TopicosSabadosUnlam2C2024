#include "SecPal.h"
#include "Palabra.h"

char* normalizar(const char* cadOrig, char* cadDest)
{
    SecPal secLect;
    SecPal secEscr;

    secPalCrear(&secLect, cadOrig);
    secPalCrear(&secEscr, cadDest);

    Palabra pal;

    secPalLeer(&secLect, &pal); //fread

    while(!secPalFin(&secLect)) //feof
    {
        palabraATitulo(&pal);
        secPalEscribir(&secEscr, &pal);  //fwrite
        if(secPalLeer(&secLect, &pal)) //fread
        {
            secPalEscribirCar(&secEscr, ' ');
        }
    }

    secPalCerrar(&secEscr); //fclose

    return cadDest;
}
