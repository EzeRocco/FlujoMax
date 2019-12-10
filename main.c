#include "flujo.h"

int main(int argc, char **argv)
{
  LNodo nuevo = NULL;
  TFlujo fl = crea_tipo_flujo();

  nuevo = toma_datos(argv[1],nuevo);
  int cant = contador(nuevo); //guardamos en cant la cantidad de elementos
//                            de la lista hasta el momento
  int arreglo[cant];
  LNodo conexiones = nuevo;
  for(;conexiones != NULL ; conexiones = conexiones->sig){
    if(conexiones->fuente == 1){
      flujo_Max(conexiones,fl,arreglo,0);
    }
  }
  
  generar_archivo(nuevo,fl,argv[2]);
  eliminar_conexiones(nuevo);
  free(fl);
  return 0;
}

