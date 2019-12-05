#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

typedef struct nodo {
  char* conex; //nombre de la conexion
  int cerrado; //1 si el camino se quedo sin capacidad/0 si quedo capacidad
  int fuente; // 1 si es la fuente / 0 si no lo es
  int sum; // 1 si es el sumidero / 0 si no lo es
  struct corriente *cor;
  struct nodo *sig; //siguien conexion
}Nodo;

typedef struct corriente{
  struct nodo *destino; //conexion a donde va ir la corriente
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

//---------------------------------------------------------------------

LNodo crea_corriente(LNodo lista, void* origen, void* llegada, int cap){
  Corriente* corr = malloc(sizeof(Corriente) + 1);
  assert(corr);
  //buscamos el nodo correspondiente a la llegada.
  LNodo ramas = lista;
  for(;ramas != NULL; ramas = ramas->sig){
    if (strcmp(ramas->conex, llegada) == 0) corr->destino = ramas;
  }
  corr->capacidadMax = cap;
  corr->capEnv = 0;
  corr->siguiente = NULL;
  //ahora guardaremos la conexion en el nodo correspondiente
  LNodo list = lista;
  for(;((list != NULL) && (strcmp(list->conex, origen) != 0)); list = list->sig);
      //una vez que accedemos al nodo correspondiente guardaremos el 
      //puntero que nos conectara al otro nodo de llegada. 
  if(strcmp(list->conex, origen) == 0){
    if (list->cor == NULL) {
      list->cor = corr;
      return lista;
    }
      
    TipCor current = list->cor;
    for(;current->siguiente != NULL;current = current->siguiente);
    current->siguiente = corr;
    return lista;
  }
  return lista;
}

LNodo crea_nodo_ini(LNodo lista,void* dato, int funSum) {
  Nodo* conexion = malloc(sizeof(Nodo)); //creamos una conexion nueva
  assert(conexion);
  conexion->conex = dato;
  (funSum == 1) ? (conexion->fuente = 1) : (conexion->fuente = 0); //si se pasa un uno(que significa que es una fuente) se marca a la fuente con uno.
  (funSum == 2) ? (conexion->sum = 1) : (conexion->sum = 0);
  if((funSum != 1) && (funSum != 2)){
    conexion->fuente = 0;
    conexion->sum = 0;
  }
  conexion->cerrado = 0;
  conexion->cor = NULL;
  conexion->sig = lista;
  return conexion;
}

TFlujo crea_tipo_flujo(){
  Flujo* f = malloc(sizeof(Flujo));
  assert(f);
  f->flujoTotal = 0;
  f->minimizador = 0;
  return f;
}

//---------------------ARCHIVO------------------------------------------
LNodo toma_datos(char* nameFile,LNodo lista){
  FILE* archivo;
  if(!(archivo = fopen(nameFile,"r"))){
   printf("\nerror al abrir el archivo llamado %s\n",nameFile);
   exit(1); 
  }

  int flag = 0;
  char line[100];
  int longitud;
  char* palabra;
  char* conectado;
  LNodo list = lista;

  while(fscanf(archivo, "%[^\n]s", line) != EOF){
    line[strcspn(line, "\r")] = '\0';
    
    if(flag == 1) {
      longitud = strlen(line);
      palabra = malloc(sizeof(char) * longitud);
      strcpy(palabra,line);
      printf("\n linea: %s",palabra);
      list = crea_nodo_ini(list,palabra,1);
      printf("\nnodo: %s",list->conex);
      flag = 0;
    }
    if(flag == 2){
      longitud = strlen(line);
      palabra = malloc(sizeof(char) * longitud);
      strcpy(palabra,line);
      printf("\n linea: %s",palabra);
      list = crea_nodo_ini(list,palabra,2);
      printf("\nnodo: %s",list->conex);
      flag = 0;
    }
    
    if(flag == 3) {
      char *temp = strtok(line, ",");
      while(temp != NULL) {
        longitud = strlen(temp);
        palabra = malloc(sizeof(char) * longitud);
        strcpy(palabra,temp);
        list = crea_nodo_ini(list,palabra,0);
        
        temp = strtok(NULL, ",");
      }
      flag = 0;
    }
    if(flag == 4) {
      char *temp2 = strtok(line,",");
      int i = 1;
      int capa;
      while(temp2 != NULL) {
        if(i == 1){
        longitud = strlen(temp2);
        palabra = malloc(sizeof(char) * longitud);
        strcpy(palabra,temp2);
       
       }
       else if(i == 2){
        longitud = strlen(temp2);
        conectado = malloc(sizeof(char) * longitud);
        strcpy(conectado,temp2);
      }
      else if(i == 3) capa = atoi(temp2);
      i++;
      temp2 = strtok(NULL, ",");
    }
    crea_corriente(list,palabra,conectado,capa);
  }
    
    
    if(strcmp(line,"Fuente") == 0){
      flag = 1;
    }
    if(strcmp(line, "Sumidero") == 0){
      flag = 2;
    }
    if(strcmp(line,"Puntos Intermedios") == 0){
      flag = 3;
    }
    if(strcmp(line,"Conexiones") == 0) {
      flag = 4;
    }

    fgetc(archivo);
  }
  fclose(archivo);
  return list;
}

void imprime_nodos (LNodo lista){
  LNodo nod = lista;
  for(;nod != NULL; nod = nod->sig){
    printf("\n%s",nod->conex);
  }
}
void imprime_conexiones(LNodo lista,char* conexion){
  LNodo nuevo = lista;
  for(; nuevo != NULL; nuevo = nuevo->sig){
    if(strcmp(nuevo->conex,conexion) == 0){
      
      //~ if(nuevo->cor == NULL){
        //~ printf("no se encontro adyacencia");
        //~ exit(1);
      //~ }
      
      TipCor aris = nuevo->cor;
      for(;aris !=NULL; aris = aris->siguiente){
        printf("\n%s esta conectado con :%s\t",conexion,aris->destino->conex);
        printf("y su capacidad es de %d",aris->capacidadMax);
        //printf(" %d\n",aris->costo);
      }
      printf("\n");
    }
  }
}

int minimo_elem(int* arreglo,int cant){
  int minimo = arreglo[0];
  printf("\n\nelemento es %d",arreglo[0]);
  for(int i = 1; i < cant; i++){
    printf("\nelemento es %d",arreglo[i]);
    if(arreglo[i] < minimo){
      printf("\n%d es menor que %d",arreglo[i],minimo);
      minimo = arreglo[i];
    }
  }
  printf("\n el minimo es %d",minimo);
  return minimo;
}

int min(LNodo lista){
  TipCor corrientes = lista->cor;
  int menor = corrientes->capacidadMax;
  for(;corrientes!= NULL; corrientes = corrientes->siguiente){
    if((corrientes->capacidadMax > menor) && (corrientes->capacidadMax != 0)) {
      printf("\nel nodo mas chico es: %d",corrientes->capacidadMax);
      menor = corrientes->capacidadMax;
    }
  }
  if(menor == 0) return -1;
  return menor;
}
///INCOMPLETA
int flujo_Max(LNodo lista,TFlujo flu,int* capacidades,int iter){
  printf("\n\nnodo inicial es: %s",lista->conex);
  int minimo = min(lista);
  int flujo = 0;
  if(minimo == -1){
    printf("\n no tiene mas nodos con capacidad");
    lista->cerrado = 1;
    return -1;
  }
  
  TipCor conectado = lista->cor;
  for(;((conectado != NULL) && (conectado->capacidadMax != minimo));conectado = conectado->siguiente);
   
  printf("\naccedemos al nodo %s ya que tiene la mayor capacidad, la capacidad es %d",conectado->destino->conex,minimo);
  printf("\nITERADOR: %d",iter);
  capacidades[iter] = conectado->capacidadMax;
  printf("\nARREGLO: %d",capacidades[iter]);
  iter++;
  if(conectado->destino->sum == 1){
    printf("\nllegamos al nodo final");
    printf("\n\nTOTAL DE ITERACIONES: %d",iter);
    flu->minimizador = minimo_elem(capacidades,iter); 
    printf("\n EL MINIMIZADOR ES %d",flu->minimizador);
    flu->flujoTotal = (flu->flujoTotal) + (flu->minimizador);
    printf("\nla capacidad maxima del nodo %s es %d y su minimo es %d",conectado->destino->conex,conectado->capacidadMax,flu->minimizador);
    conectado->capacidadMax = (conectado->capacidadMax) - (flu->minimizador);
    printf("\nahora la capacidad queda en: %d",conectado->capacidadMax);
    conectado->capEnv = (conectado->capEnv) + (flu->minimizador);
    return flujo = flu->minimizador;
  }
        
  else if(conectado->destino->sum != 1){
    LNodo conexion = conectado->destino;
    printf("\n el destino es # %s",conexion->conex);
    flujo = flujo_Max(conexion,flu,capacidades,iter);
    printf("\nvolvemos al nodo : #%s",lista->conex);

    if(flujo == -1){
      printf("\n entramos en zona -1");
      for(;((conectado != NULL));conectado = conectado->siguiente){
        if(conectado->destino->cerrado != 1){
          LNodo conexion2 = conectado->destino;
          capacidades[iter - 1] =conectado->capacidadMax;
          flujo = flujo_Max(conexion2,flu,capacidades,iter);
          printf("\nQUIERO SABER EL FLUJO: %d",flujo);
        }
      if(flujo != -1){ //si tengo minimizador hago las operaciones correspondientes
        conectado->capacidadMax = (conectado->capacidadMax) - flujo;
        printf("\n \nahora la capacidad queda en: %d",conectado->capacidadMax);
        conectado->capEnv = (conectado->capEnv) + flujo;
        return flujo;
    }
      }
    }
    else if(flujo != -1){ //si tengo minimizador hago las operaciones correspondientes
      conectado->capacidadMax = (conectado->capacidadMax) - flujo;
      printf("\n \nahora la capacidad queda en: %d",conectado->capacidadMax);
      conectado->capEnv = (conectado->capEnv) + flujo;
    }
      //~ if(conexion->cerrado == 1) return -1;
      
    //~ }
  }

  if((lista->fuente == 1) && (flujo != -1)){
    printf("\n\nLLEGAMOS AL NODO INICIAL");
    imprime_conexiones(lista,"1");
    flujo = flujo_Max(lista,flu,capacidades,0);
  }

  return flujo;
}



int main()
{
  LNodo nuevo = NULL;
  TFlujo fl = crea_tipo_flujo();
  //~ nuevo = crea_nodo_ini(nuevo,"1",1);
  //~ imprime_nodos (nuevo);
  //~ nuevo = crea_nodo_ini(nuevo,"2",0);
  //~ nuevo = crea_nodo_ini(nuevo,"3",2);

  nuevo = toma_datos("fuentes.txt",nuevo);
  //nuevo = crea_corriente(nuevo,"1","2",5);
  //printf("\n%s",nuevo->cor->destino->conex);
  //imprime_nodos(nuevo);
  //~ nuevo = crea_corriente(nuevo,"1","2",5);
  //~ nuevo = crea_corriente(nuevo,"1","3",5);
  imprime_conexiones(nuevo,"1");
  imprime_conexiones(nuevo,"3");
  int arreglo[10];
  LNodo conexiones = nuevo;
  for(;conexiones != NULL; conexiones = conexiones->sig){
    if(conexiones->fuente == 1){
      flujo_Max(conexiones,fl,arreglo,0);
    }
  }
  printf("\nEL FLUJO MAXIMO ES: %d",fl->flujoTotal);
  return 0;
}

