#include "flujo.h"

//---------------------------------------------------------------------
//~ crea_corriente: LNodo -> void* -> void* -> int -> LNodo
//~ la funcion crea_corriente toma una lista de LNodo(donde ya se encuentran todos los nodos)
//~ dos void*(que pueden ser char*),uno origen donde nos podra ayudar a identificar el nodo a donde
//~ se va a guardar la adyacencia , y otro de llegada donde podremos identificar a donde podra acceder
//~ esa adyacencia,por ultimo un entero con la capacidad que que guardara en capacidadOriginal.La funcion
//~ nos devolvera la lista pero ahora con la adyacencia ya guardada en la lista. 
LNodo crea_corriente(LNodo lista, void* origen, void* llegada, int cap){
  Corriente* corr = malloc(sizeof(Corriente) + 1);
  assert(corr);
  //buscamos el nodo correspondiente a la llegada.
  LNodo ramas = lista;
  for (;ramas != NULL; ramas = ramas->sig) {
    if (strcmp(ramas->conex, llegada) == 0) corr->destino = ramas;
  }
  corr->capacidadOriginal = cap;
  corr->capacidadMax = corr->capacidadOriginal; //guardaremos lo mismo que esta en corr->capacidadOriginal ya que los dos
//                                                empezaran con la misma capacidad
  corr->capEnv = 0;
  corr->siguiente = NULL;
  //ahora guardaremos la conexion en el nodo correspondiente
  LNodo list = lista;
  for (;((list != NULL) && (strcmp(list->conex, origen) != 0)); list = list->sig);
      //una vez que accedemos al nodo correspondiente guardaremos el 
      //puntero que nos conectara al otro nodo de llegada. 
  if(strcmp(list->conex, origen) == 0){
    if (list->cor == NULL) {
      list->cor = corr;
      return lista;
    }
      
    TipCor current = list->cor;
    for (;current->siguiente != NULL;current = current->siguiente);
    current->siguiente = corr;
  }
  return lista;
}

//~ crea_nodo_ini: LNodo -> void* -> int -> LNodo
//~ la funcion crea_nodo_ini toma una lista de tipo LNodo, un void* con el dato(nombre de la conexion)
//~ que sera agregado a la lista, y un entero que nos ayudara a identificar si es la fuente o el sumidero
//~ o si no es de ninuno de los dos. La funcion nos devolvera la lista pero ahora con este nuevo nodo agre-
//~ gado al principio de la lista. 

LNodo crea_nodo_ini(LNodo lista,void* dato, int funSum) {
  Nodo* conexion = malloc(sizeof(Nodo)); //creamos una conexion nueva
  assert(conexion);
  conexion->conex = dato;
  conexion->caminado = 0;
  //si funSum es uno(que significa que es una fuente) se marca a la fuente con uno.
  (funSum == 1) ? (conexion->fuente = 1) : (conexion->fuente = 0);
  //si funSum es dos el sumidero se marca en uno,sino se marca en cero.
  (funSum == 2) ? (conexion->sum = 1) : (conexion->sum = 0);
  //si funSum no es ni uno ni dos ,la fuente como el sumidero se marcaran en cero.
  if((funSum != 1) && (funSum != 2)){
    conexion->fuente = 0;
    conexion->sum = 0;
  }
  conexion->cerrado = 0;
  conexion->cor = NULL;
  conexion->sig = lista;
  return conexion;
}

//~ crea_tipo_flujo: void -> TFlujo
//~ la funcion crea_tipo_flujo nos inicializa la estructura Flujo con sus valores
//~ en cero.

TFlujo crea_tipo_flujo() {
  Flujo* f = malloc(sizeof(Flujo));
  assert(f);
  f->flujoTotal = 0;
  f->minimizador = 0;
  return f;
}

//---------------------ARCHIVO------------------------------------------
//~ toma_datos: char* -> LNodo -> LNodo
//~ la funcion toma_datos abre el archivo donde se encuentran los datos y lo
//~ que se encuentra en fuente,sumidero y puntos intermedios lo guardamos en
//~ la funcion crea_nodo_ini y lo que se encuentra en conexiones lo guardamos
//~ en la funcion crea_corriente.
LNodo toma_datos(char* nameFile,LNodo lista){
  FILE* archivo;
  if(!(archivo = fopen(nameFile,"r"))) {
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
    
    if(flag == 1) { //si es 1 guardaremos en la lista al nodo como fuente
      longitud = strlen(line);
      palabra = malloc(sizeof(char) * longitud);
      strcpy(palabra,line);
      list = crea_nodo_ini(list,palabra,1);
      flag = 0;
    }
    if(flag == 2){ //si es 2 guardaremos en la lista al nodo como sumidero
      longitud = strlen(line);
      palabra = malloc(sizeof(char) * longitud);
      strcpy(palabra,line);
      list = crea_nodo_ini(list,palabra,2);
      flag = 0;
    }
    
    if(flag == 3) { //si es 3 guardaremos los demas nodos en la lista
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
    if(flag == 4) { //si es 4 guardaremos las adaycencias con su capacidades correspondientes
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
    if(strcmp(line,"Puntos Intermedios") == 0) {
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

//~ generar_archivo: LNodo -> TFlujo -> void
//~ generar_archivo toma la lista de tipo LNodo resultante de la funcion
//~ flujo_Max ,como tambien el flujoTotal de TFlujo y nos genera un nuevo
//~ archivo con el flujo total y las conexiones con sus capacidad de envio correspondientes.
void generar_archivo(LNodo lista,TFlujo flujoTot,char*nombreArchivo){
  FILE* archivo;
  if(!(archivo = fopen(nombreArchivo,"w"))) {
    printf("ERROR al abrir el archivo camino");
    exit(1);
  }
  fprintf(archivo,"Flujo maximo:\n%d\nFlujos\n",flujoTot->flujoTotal);
  LNodo nods = lista;
  for (;nods != NULL; nods = nods->sig) {
    if(nods->cor != NULL) {
      TipCor cur = nods->cor;
      for (;cur != NULL; cur = cur->siguiente) {
        fprintf(archivo,"%s,%s,%d,%d\n",nods->conex,cur->destino->conex,cur->capacidadOriginal,cur->capEnv);
      }
    }
  }
  fclose(archivo);
}

//------------------------Eliminacion---------------------------------
//~ eliminar_corriente: LNodo -> void
//~ la funcion eliminar_corriente libera la memoria pedida para las 
//~ adaycencias del nodo que se pasa como argumento. 
void eliminar_corriente(LNodo listCor){
  TipCor corr = listCor->cor;
  TipCor corrTemp;
  while (corr != NULL) {
    corrTemp = corr;
    corr = corr->siguiente;
    free(corrTemp);
  }
  free(corr);
}

//~ eliminar_conexiones: LNodo -> void
//~ la funcion eliminar_conexiones libera la memoria pedida para todos los
//~ nodos que se encuentran en la lista.

void eliminar_conexiones(LNodo lista){
  LNodo list = lista;
  LNodo listTemp;
  while (list != NULL) {
    listTemp = list;
    list = list->sig;
    free(listTemp->conex);
    if(listTemp->cor != NULL) {
      eliminar_corriente(listTemp);
    }
    free(listTemp);
  }
  free(list);
}
// -----------------------Funciones-------------------------------------
//~ contador: LNodo -> int
//~ la funcion contador toma una lista de tipo LNodo y nos devuelve la cantidad 
//~ de elementos que posee la lista.
int contador(LNodo list){
  int cont = 0;
  LNodo lis = list;
  for (;lis != NULL; lis = lis->sig) {
    cont++;
  }
  return cont;
} 

//~ minimo_elem: int*-> cant
//~ dado un arreglo de enteros y la cantidad de elemento que posee dicho arreglo,
//~ nos devuelve el menor elemento que se encuentra en el arreglo hasta el momento.

int minimo_elem(int* arreglo,int cant){
  int minimo = arreglo[0];
  //printf("\n\nelemento es %d",arreglo[0]);
  for (int i = 1; i < cant; i++) {
    //printf("\nelemento es %d",arreglo[i]);
    if(arreglo[i] < minimo) {
      //printf("\n%d es menor que %d",arreglo[i],minimo);
      minimo = arreglo[i];
    }
  }
  //printf("\n el minimo es %d",minimo);
  return minimo;
}

//~ reset: LNodo -> void
//~ la funcion reset no vuelve a dejar el nodo sin caminar,todos los nodos
//~ que se encuentran con caminado en 1 ,vuelve a ponerlo en 0 para poder asi
//~ recorrer nuevamente los nodos.

void reset(LNodo lista){
  LNodo list = lista;
  for (;list != NULL;list = list->sig) {
    list->caminado = 0;
  }
}

//~ max:LNodo -> int
//~ la funcion max nos devuelve la cantidadMax mas grande de las adyacencias
//~ de cierto nodo.

int max(LNodo lista){
  TipCor corrientes = lista->cor;
  int mayor = 0;
  for (;corrientes!= NULL; corrientes = corrientes->siguiente) {
    if((corrientes->capacidadMax > mayor) && (corrientes->destino->caminado != 1)) {
      //printf("\nel nodo mas chico es: %d",corrientes->capacidadMax);
      mayor = corrientes->capacidadMax;
    }
  }
  return mayor;
}

//~ flujo_Max: LNodo -> TFlujo -> int* -> int -> int
//~ la funcion de flujo_Max toma una lista de tipo LNodo , un flujo de tipo TFlujo,
//~ una arreglo de enteros donde guardaremos las distintas capacidades que encontremos
//~ mediante recorramos la lista y un entero que servira como indice para iterar.
//~ La funcion nos guardar el flujo maximo que obtenemos al haber recorrido todas las
//~ capacidades.

int flujo_Max(LNodo lista,TFlujo flu,int* capacidades,int iter){
  int maximo = max(lista);
  int flujo = 0;
  if(maximo == 0) { //si el maximo es cero marcamos ese camino como cerrado y devolvemos -1.
    lista->cerrado = 1;
    return -1;
  }
  lista->caminado = 1; //marcamos como caminado el nodo.
  TipCor conectado = lista->cor; //tomamos las adyacencias.
  for (;((conectado != NULL) && (conectado->capacidadMax != maximo));conectado = conectado->siguiente);
   
  capacidades[iter] = conectado->capacidadMax; //guardamos la capacidad maxima en el arreglo.
  iter++;
  if(conectado->destino->sum == 1) { //si se llego al ultimo nodo(sumidero) -
//guardamos el minimo y el flujo total, asi tambien retornando el minimo elemento
    flu->minimizador = minimo_elem(capacidades,iter); 
    flu->flujoTotal = (flu->flujoTotal) + (flu->minimizador);
    conectado->capacidadMax = (conectado->capacidadMax) - (flu->minimizador);
    conectado->capEnv = (conectado->capEnv) + (flu->minimizador);
    reset(lista); //volvemos a dejar en cero todos los nodos caminados
    flujo = flu->minimizador;
    return flujo;
  }
  //si no el nodo no es el sumidero ni tampoco fue caminado ,hago un llamado
  //recursivo para acceder a otro nodo.
  else if((conectado->destino->sum != 1) && (conectado->destino->caminado != 1)) {
    LNodo conexion = conectado->destino;
    flujo = flujo_Max(conexion,flu,capacidades,iter);

    if(flujo == -1) {
      //buscamos algun nodo que no este caminado ni cerrado
      for (;((conectado != NULL));conectado = conectado->siguiente) {
        if((conectado->destino->cerrado != 1) && (conectado->destino->caminado != 1)) {
          LNodo conexion2 = conectado->destino;
          capacidades[iter - 1] =conectado->capacidadMax; //guardamos en el arreglo la capacidad maxima
          flujo = flujo_Max(conexion2,flu,capacidades,iter);
        }
      //si este flujo nuevo que estabamos buscando es distinto de menos uno 
      //la capacidad maxima y la capacidad enviada con el flujo que tenemos 
      //hasta el momento  y devolvemos inmediatamente el flujo.
        if(flujo != -1) { //si tengo minimizador hago las operaciones correspondientes
        conectado->capacidadMax = (conectado->capacidadMax) - flujo;
        conectado->capEnv = (conectado->capEnv) + flujo;
        return flujo;
        }
      }
    }
    //si el flujo es distinto de menos uno, guardamos la capacidad maxima y la capacidad
    //enviada con el flujo que tenemos hasta el momento.
    else if(flujo != -1){ //si tengo minimizador hago las operaciones correspondientes
      conectado->capacidadMax = (conectado->capacidadMax) - flujo;
      conectado->capEnv = (conectado->capEnv) + flujo;
    }
  }
  //si volvimos al nodo inicial(fuente) y su flujo es distinto de menos uno
  //hacemos un llamado recursivo para accerder a otro nodo ya que todavia
  //podriamos tener caminos que nos lleven al sumidero.
  if((lista->fuente == 1) && (flujo != -1)){
    flujo = flujo_Max(lista,flu,capacidades,0);
  }

  return flujo;
}


