#ifndef __FLUJO_H__
#define __FLUJO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

typedef struct nodo {
  char* conex; //nombre de la conexion
  int caminado; //1 si ya accedimos a este nodo anteriormente
  int cerrado; //1 si el camino se quedo sin capacidad/0 si quedo capacidad
  int fuente; // 1 si es la fuente / 0 si no lo es
  int sum; // 1 si es el sumidero / 0 si no lo es
  struct corriente *cor;
  struct nodo *sig; //siguien conexion
}Nodo;

typedef struct corriente{
  struct nodo *destino; //conexion a donde va ir la corriente
  int capacidadOriginal;
  int capacidadMax; //capacidad Maxima de la conexion
  int capEnv; // capacidad que se envia(todos comienzan en cero)
  struct corriente *siguiente;
}Corriente;

typedef Corriente *TipCor;
typedef Nodo *LNodo;

typedef struct _fl{
  int flujoTotal;
  int minimizador;
}Flujo;

typedef Flujo *TFlujo;

LNodo crea_corriente(LNodo lista, void* origen, void* llegada, int cap);

LNodo crea_nodo_ini(LNodo lista,void* dato, int funSum);

TFlujo crea_tipo_flujo();

//ARCHIVOS
LNodo toma_datos(char* nameFile,LNodo lista);

void generar_archivo(LNodo lista,TFlujo flujoTot,char*nombreArchivo);

// LIBERACION DE MEMORIA
void eliminar_corriente(LNodo listCor);

void eliminar_conexiones(LNodo lista);

// FUNCIONES
int contador(LNodo list);

int minimo_elem(int* arreglo,int cant);

void reset(LNodo lista);

int max(LNodo lista);

int flujo_Max(LNodo lista,TFlujo flu,int* capacidades,int iter);

#endif /* __FLUJO_H__ */
