
#include <stdio.h>


int flujo_Max(LNodo lista,TFlujo flu,int* capacidades,int iter){
  printf("\n\nnodo inicial es: %s",lista->conex);
  int minimo = min(lista);
  printf("\nla menor conexion de %s es %d",lista->conex,minimo);
  TipCor conectado = lista->cor;
  for(;conectado != NULL; conectado = conectado->siguiente){
    if((conectado->capacidadMax == minimo) && (conectado->capacidadMax != 0)){
      printf("\naccedemos al nodo %s ya que tiene la menor conexion",conectado->destino->conex);
      printf("\nITERADOR: %d",iter);
      capacidades[iter] = conectado->capacidadMax;
      printf("\nARREGLO: %d",capacidades[iter]);
      iter++;
      
      if(conectado->destino->sum != 1){
        LNodo conexion = conectado->destino;
        int flujo = flujo_Max(conexion,flu,capacidades,iter);
        printf("\nla capacidad maxima del nodo %s es %d y su minimo es %d",conectado->destino->conex,conectado->capacidadMax,flu->minimizador);
        conectado->capacidadMax = (conectado->capacidadMax) - (flu->minimizador);
        printf("\nahora la capacidad queda en: %d",conectado->capacidadMax);
        conectado->capEnv = (conectado->capEnv) + (flu->minimizador);
        return flujo;
      }
      
      else{
        printf("\nllegamos al nodo final");
        printf("\n\nTOTAL DE ITERACIONES: %d",iter);
        flu->minimizador = minimo_elem(capacidades,iter); 
        printf("\n EL MINIMIZADOR ES %d",flu->minimizador);
        flu->flujoTotal = (flu->flujoTotal) + (flu->minimizador);
        printf("\nla capacidad maxima del nodo %s es %d y su minimo es %d",conectado->destino->conex,conectado->capacidadMax,flu->minimizador);
        conectado->capacidadMax = (conectado->capacidadMax) - (flu->minimizador);
        printf("\nahora la capacidad queda en: %d",conectado->capacidadMax);
        conectado->capEnv = (conectado->capEnv) + (flu->minimizador);
        return flu->flujoTotal;
      }
    }
  }
  //~ if((lista->fuente == 1) )
    //~ flujo_Max(lista,flu,capacidades,0);
  return 1;
}

int main(int argc, char **argv)
{
	
	return 0;
}

