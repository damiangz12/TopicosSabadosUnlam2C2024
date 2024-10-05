#include "SecPal.h"
#include "Palabra.h"


char* normalizar(const char* cadOrig, char* cadDest)
{
    SecPal secLect;
    SecPal secEscr;

    secPalCrear(&secLect, cadOrig);
    secPalCrear(&secEscr, cadDest);

    Palabra pal;
	
    secPalLeer(&secLect, &pal);
    while(!secPalFin(&secLect))
    {
      palabraATitulo(&pal);
      secPalEscribir(&secEscr, &pal);
      if(secPalLeer(&secLect, &pal))
      {
        secPalEscribirCar(&secEscr, ' ');
      }
    }

	secPalCerrar(&secEscr);

	return cadDest;
}
