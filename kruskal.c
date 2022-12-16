#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int tipo_nombre;
typedef int tipo_elemento;
typedef int vertice;

#define VERTICES 5


// Forma en la que representamos una arista
typedef struct _ARISTA{
    vertice u;
    vertice v;
    int costo;
} arista;

// La estructura rama es una lista enlazada de aristas que vamos a utilizar para poder realizar el algoritmo
typedef struct _RAMA{
    struct _ARISTA a;
    struct _RAMA * sig;
} rama;

typedef struct _ENCABEZADO{
    int cuenta;         // cantidad de vertices que tiene la componente
    int primer_elemento;// cual es el primer vertice de la componente
} encabezado;

typedef struct _NOMBRE{
    tipo_nombre nombre_conjunto; // nombre de la componente a la que pertence
    int siguiente_elemento;     // indice del siguiente vertice que pertenece a la componente
} nombre;

// Esta estructura la usamos para representar el arbol que buscamos
// primero es un grafo que contiene a todos los vertices del grafo original, pero no tiene ninguna arista
// (agregamos los vertices a esta estructura utilizando la funcion inicial)
typedef struct _CONJUNTO_CE{
    nombre nombres [VERTICES];
    encabezado encabezamientos_conjunto [VERTICES];
} conjunto_CE;


void inicial(tipo_nombre, tipo_elemento, conjunto_CE *);
void combina(tipo_nombre, tipo_nombre, conjunto_CE *);
void fusionar(tipo_nombre , tipo_nombre , conjunto_CE *);
tipo_nombre encuentra (int, conjunto_CE *);
void kruskal (rama **);
void inserta (int, int, int, rama **);
arista sacar_min (rama **);
void lista (rama *);

//Funciones de formato
void separador();
void verde();
void rojo();
void amarillo();
void blanco();
void negro();
void colorReset();
void header();
void limpiarPantalla(); 
void bufferEnter();


int main ()
{
    header(); 
    printf("\n");
    int M_Costos[VERTICES][VERTICES] = {0};
    rama * arbol = NULL;
    printf("Ingrese costo de lado entre vertices... \n"); 

    for (int i = 0; i <= VERTICES -1; i++){
        for (int j = i + 1; j <= VERTICES -1; j++){
            amarillo(); printf("%d", i); colorReset(); 
            printf(" y "); 
            amarillo(); printf("%d", j); colorReset();
            printf(": ");
            //Validacion de ingreso
            int ingreso = 0, salir = 0;
            while(salir == 0){
                verde(); scanf("%d", &ingreso);
                if(ingreso <= 0){
                    rojo(); printf("Ingreso invalido.\n"); colorReset();
                    printf("Ingrese nuevamente: ");
                } else {
                    salir = 1;
                }
            }
            M_Costos[i][j] = ingreso;
            colorReset();
        }
    }

    // la mitad inf. de diagonal de matriz;
    // lo que significa esto es que solo vamos a cargar una arista que cumpla lo siguiente:
    // - No aparecio (incluye las aristas que tienen el origne y el final invertidos)
    // - El incio de la arista no coincide con el final de la misma
    for(int i = 0; i <= VERTICES -1; i ++){
        for(int j = i + 1; j <= VERTICES -1; j++){
            if(M_Costos [i][j] != 0){
                inserta(i, j, M_Costos[i][j], &arbol); //inserto en cola prior .
            }
        }
    }

    kruskal(&arbol);
    printf("La informacion del arbol minimo para el grafo ingresado resulta: ");
    lista(arbol);
    return 0;
}

// Dado los vertices de una arista y un valor inserta la arista en la lista enlazada dada
void inserta(int vA, int vB, int costo, rama ** arbol){
    rama * nuevaRama = (rama*) malloc(sizeof(rama));

    arista nuevaArista;
    nuevaArista.u = vA;
    nuevaArista.v = vB;
    nuevaArista.costo = costo;

    nuevaRama->a = nuevaArista;
    nuevaRama->sig = NULL;
    
    if(*arbol == NULL) {
        *arbol = nuevaRama;
    } else {
        rama * aux = *arbol;
        while(aux -> sig!= NULL){
            aux = aux -> sig;
        }
        aux->sig = nuevaRama;
    }
}

void kruskal(rama ** arbol){
    int comp_n, comp_siguiente, comp_u, comp_v;
    tipo_elemento u, v;
    conjunto_CE *componentes = (conjunto_CE*) malloc(sizeof(conjunto_CE));
    rama * resultado = NULL;
    comp_siguiente = 0;
    comp_n = VERTICES;
    arista arista_minima;

    //Inicialmente, por cada vertice, generamos una nueva componente de un unico vertice y sin siguiente
    for(int i = 0; i<VERTICES; i++){
        inicial(comp_siguiente, i, componentes);
        comp_siguiente++;
    }

    while(comp_n > 1){
        arista_minima = sacar_min(arbol);
        u = arista_minima.u;
        v = arista_minima.v;
        comp_u = encuentra(u, componentes);
        comp_v = encuentra(v, componentes);

        if(comp_u != comp_v){
            combina(comp_u, comp_v, componentes);
            comp_n--;
            inserta(arista_minima.u, arista_minima.v, arista_minima.costo, &resultado);
        }
    }

    //Se libera el arbol original
    rama * aux = *arbol;
    while(aux != NULL){
        *arbol = aux;
        aux = (*arbol)->sig;
        free(*arbol);
    }
    //Se reasigna al nuevo reusltado del kruskal.
    *arbol = resultado;
}


// La funcion recibe un nombre para una componenete y un nombre de vertice y lo agrega a el conjunto
// La idea de esta funcion es que vos primero metes a todos los vertices en el conjunto y como todavia no los conectaste entonces
// el verticce suelto es una componente en si mimso
void inicial(tipo_nombre componente, tipo_elemento x, conjunto_CE * conjunto) {
    (conjunto->nombres)[x].nombre_conjunto = componente;
    (conjunto->nombres)[x].siguiente_elemento = -1; // como apenas se agrega la componente al grafo solo tiene un vertice y no existe el siguiente
    (conjunto->encabezamientos_conjunto)[componente].cuenta = 1;
    (conjunto->encabezamientos_conjunto)[componente].primer_elemento = x;
}


// Esta funcion recibe los nombres de dos componenetes que pertencen a el tercer conjunto dado y
// combina estas dos componenetes (el nombre de la nueva componente es el de una de las dos fusionadas)
void combina(tipo_nombre comp_A, tipo_nombre comp_B, conjunto_CE * conjunto){
    int cant_a = (conjunto->encabezamientos_conjunto)[comp_A].cuenta;
    int cant_b = (conjunto->encabezamientos_conjunto)[comp_B].cuenta;
    if(cant_a > cant_b){// si la componente a tiene mas elementos que la componente b entonces mete el contenido de la b en la a
        fusionar(comp_A, comp_B, conjunto);
    } else {
        fusionar(comp_B, comp_A, conjunto);
    }
}


// dadas dos componentes de un conjunto convierte todos los vertices de la segunda componente en vertices de la primera
//  una funcion para hacer mas legible y no repetir el codigo en la funcion combina
void fusionar(tipo_nombre comp_A, tipo_nombre comp_B, conjunto_CE * conjunto){
    tipo_elemento elem = (conjunto->encabezamientos_conjunto)[comp_B].primer_elemento;
        // minetras no se haya encontrado el último elemento de la lista B
        while((conjunto->nombres)[elem].siguiente_elemento != -1){
            (conjunto->nombres)[elem].nombre_conjunto = comp_A;
            elem = (conjunto->nombres)[elem].siguiente_elemento;
        }
        // ahora mismo elem contiene el ultimo elemento de la nueva componente
        (conjunto->nombres)[elem].nombre_conjunto = comp_A;
        (conjunto->nombres)[elem].siguiente_elemento = (conjunto->encabezamientos_conjunto)[comp_A].primer_elemento;
        (conjunto->encabezamientos_conjunto)[comp_A].primer_elemento = (conjunto->encabezamientos_conjunto)[comp_B].primer_elemento;
        (conjunto->encabezamientos_conjunto)[comp_A].cuenta += (conjunto->encabezamientos_conjunto)[comp_B].cuenta;
}


// La funcion recibe un vertice y un conjunto de componentes de aristas
// Lo que hace es devolver el nombre de la componente a la que pertenece el vertice
tipo_nombre encuentra(int vertice, conjunto_CE * conjunto){
    return ((conjunto->nombres)[vertice].nombre_conjunto);
}



// Dada una lista enlazada de aristas, devuelve la arista de menor costo dentro de la lista
arista sacar_min(rama ** arbol){
    arista costo_min = (*arbol)->a; //obtenemos la primera arista de la lista
    rama * auxiliar = *arbol;
    while(auxiliar != NULL){
        if(auxiliar->a.costo < costo_min.costo){
            costo_min = auxiliar->a;
        }
        auxiliar = auxiliar->sig;
    }
    auxiliar = *arbol;
    //buscamos el elemento previo a la arista de menor costo
    //en caso de que el primer elemento sea el de menor costo lo borramos sin realizar otro cambio
    if(auxiliar->a.costo == costo_min.costo){
        *arbol = (*arbol)->sig;
        free(auxiliar); // contiene al primer elemento
    }else{
        while(auxiliar->sig->a.costo != costo_min.costo){
            auxiliar = auxiliar->sig;
        }
        rama * borrar = auxiliar->sig;
        auxiliar->sig = auxiliar->sig->sig;//al elemento previo del elemento a borrar  lo anexamos con el siguiente al que se va a borrar
        free(borrar);
    }
    return costo_min;
}



void lista(rama * arbol){
    printf("\n");
    rama * aux = arbol;
    while(aux != NULL){
        //printf("(%i - %i: %i)", aux->a.u, aux->a.v, aux->a.costo);
        rojo();printf("(");colorReset();
        amarillo(); printf("%i", aux->a.u);
        negro(); printf(" - "); colorReset();
        amarillo(); printf("%i", aux->a.v);
        negro(); printf(" : "); colorReset();
        verde(); printf("%i", aux->a.costo);
        rojo();printf(")");printf(" -> "); colorReset();
        aux = aux -> sig;
    } 
    negro(); printf(" NULL (Fin)\n"); colorReset();
}





void separador(){
    negro();
    printf("---------------------------------\n");
    colorReset();
}

void header(){
    verde(); printf("--> Kruskal <--\n");
    separador();
    printf("Vertices trabajados : "); 
    amarillo(); printf("%d\n", VERTICES);
    separador();
    printf("Ingrese los pesos de las aristas entre los vertices para formar el grafo, \nel programa tomara ese grafo y devolvera un listado\nde las aristas que conforman su arbol minimo\ndeterminado por el algoritmo de kruskal.\n");
    separador();
    negro(); printf("Andres Grillo - Teo Baiocchi - AUS 2022 T.P.II\n");
    separador();
}

void verde(){
    printf("\033[1;32m");
}

void rojo(){
    printf("\033[1;31m");
}

void amarillo(){
    printf("\033[1;33m");
}

void blanco(){
    printf("\033[1;37m");
}
void negro(){
    printf("\033[1;30m");
}

void colorReset(){
    printf("\033[0m");
}


void limpiarPantalla(){
  #if defined(linux) || defined(unix) || defined(APPLE)
    system("clear");
  #endif

  #if defined(_WIN32) || defined(_WIN64)
    system("cls");
  #endif
 }


void bufferEnter(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF){}
}
