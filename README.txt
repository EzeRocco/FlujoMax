ESTRUCTURAS DE DATOS Y ALGORITMOS 1.
TRABAJO PRACTICO FINAL:
HECHO POR: Rocco Ezequiel

ARCHIVOS ENTREGADOS:
    * main.c
    * flujo.c
    * flujo.h
    
    * CARPETA CONTENEDORA CON DISTINTOS EJEMPLOS(universos)
    * README


COMPILACIÓN:
En el directorio del archivo:
    gcc -c flujo.c
    gcc -o main main.c flujo.o
    ./main NombreArchivoEntrada.txt NombreArchivoSalida.txt
    
ARCHIVOS .txt PARA LA EJECUCION:
el usuario debera pasara antes de la ejecucion un .txt de entrada con el universo que quiera probar el programa
(el nombre va en donde dice NombreArchivoEntrada) y el nombre que
le quiera poner al .txt de salida(igual en NombreArchivoSalida)

OPINION DEL TRABAJO PRACTICO Y ACLARACIONES DEL PROGRAMA:
*Mi opinion sobre el trabajo practico es muy buena ya que pude aprender de algunos errores que tuve
en el anterior trabajo.
*Volvi aplicar lista de adyacencias ya que para mi guardar y acceder a los elementos con su adyacencia
correspondiente me resulta facil al igual tambien con el trabajo de bactracking.
*el universo que se brinde debera tener (al igual que el ejemplo de comunidades) las identificaciones corres-
pondientes con la primera letra en mayuscula,ejemplo:Fuente,Sumidero,Puntos Intermedios,Conexiones.Las conexiones
deberan estar separadas por comas y sin espacios en blancos.
*La manera de recorrer los nodos es:
 --Primero desde el primer nodo accedemos a la adyacencia con mas capacidad
hasta el momento y recorreremos los nodos con la misma metodologia y guardaremos 
esas capacidades en el arrego de enteros que pasamos como argumento.
 --Cuando se llega al final se busca la MENOR capacidad del arreglo y lo guardaremos
provisoriamente en flujoTotal y minimizador (ambos de TFlujo),tambien devolveremos
este minimizador.
 --la funcion terminara cuando se encuentre en el nodo fuente y no tenga mas capacidad. 
