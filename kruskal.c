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
    int cuenta;// cantidad de vertices que tiene la componente
    int primer_elemento;// cual es el primer vertice de la componente
} encabezado;

typedef struct _NOMBRE{
    tipo_nombre nombre_conjunto; // nombre de la componente a la que pertence
    int siguiente_elemento; // indice del siguiente vertice que pertenece a la componente
} nombre;


// Esta estructura la usamos para representar el arbol que buscamos
// primero es un grafo que contiene a todos los vertices del grafo original, pero no tiene ninguna arista
// (agregamos los vertices a esta estructura utilizando la funcion inicial)
typedef struct _CONJUNTO_CE{
    nombre nombres [VERTICES];
    encabezado encabezamientos_conjunto [VERTICES];
} conjunto_CE;

// La funcion recibe un nombre para una componenete y un nombre de vertice y lo agrega a el conjunto 
// La idea de esta funcion es que vos primero metes a todos los vertices en el conjunto y como todavia no los conectaste entonces
// el verticce suelto es una componente en si mimso
void inicial(tipo_nombre, tipo_elemento, conjunto_CE *);

// Esta funcion recibe los nombres de dos componenetes que pertencen a el tercer conjunto dado y
// combina estas dos componenetes (el nombre de la nueva componente es el de una de las dos fusionadas)
void combina(tipo_nombre, tipo_nombre, conjunto_CE *);

// La funcion recibe un vertice y un conjunto de componentes de aristas
// Lo que hace es devolver el nombre de la componente a la que pertenece el vertice
tipo_nombre encuentra (int, conjunto_CE *);

void kruskal (rama *);

// Dado los vertices de una arista y un valor inserta la arista en la lista enlazada dada 
void inserta (int, int, int, rama **);

// Dada una lista enlazada de aristas, devuelve la arista de menor costo dentro de la lista
arista * sacar_min (rama **);

// con total honestidad no tengo la mas palida idea de que hace esto
// lo unico que se me cruza por la cabeza es que esto muestre el resultado al final 
// (por lo que venimos teniendo con Pablo siempre pide una funcion de este tipo)
void lista (rama *);

//  una funcion para hacer mas legible y no repetir el codigo en la funcion combina
void fusionar(tipo_nombre , tipo_nombre , conjunto_CE *);

int main ()
{
    int M_Costos [VERTICES][VERTICES];
    rama * arbol = (rama*)malloc(sizeof(rama));

    for (int i = 0; i <= VERTICES -1; i++){
        for (int j = i + 1; j <= VERTICES -1; j++){
            printf("Ingrese costo de lado entre vertices %d y %d : ", i, j) ;
            scanf("%d", &M_Costos [i][j]);
        }
    }

    for(int i = 0; i <= VERTICES -1; i ++){
    // la mitad inf. de diagonal de matriz;
    // lo que significa esto es que solo vamos a cargar una arista que cumpla lo siguiente:
    // - No aparecio (incluye las aristas que tienen el origne y el final invertidos)
    // - El incio de la arista no coincide con el final de la misma
        for(int j = i + 1; j <= VERTICES -1; j++){
            if(M_Costos [i][j] != 0){
                inserta(i, j, M_Costos[i][j], &arbol); //inserto en cola prior .
            }
        }
    }

    printf("Representacion de la matriz ingresada: ");
    for(int i = 0; i < VERTICES; i++){
        for(int j = 0; j < VERTICES; j++){
            printf("- %i ", M_Costos[i][j]);
        }
        printf("\n");
    }
        

    printf("xD\n");

    //kruskal(arbol);
    return 0;
}



void inserta(int vA, int vB, int costo, rama ** arbol){
    rama * nuevaRama = (rama*) malloc(sizeof(rama));

    arista nuevaArista;
    nuevaArista.u = vA;
    nuevaArista.v = vB;
    nuevaArista.costo = costo;

    nuevaRama->a = nuevaArista;
    nuevaRama->sig = NULL;

    (*arbol)->sig = nuevaRama;
}
       
    
void inicial(tipo_nombre componente, tipo_elemento x, conjunto_CE * conjunto){
    (conjunto->nombres)[x].nombre_conjunto = componente;
    (conjunto->nombres)[x].siguiente_elemento = 0; // como apenas se agrega la componente al grafo solo tiene un vertice y no existe el siguiente
    (conjunto->encabezamientos_conjunto)[componente].cuenta = 1;
    (conjunto->encabezamientos_conjunto)[componente].primer_elemento = x;
}

void combina(tipo_nombre comp_A, tipo_nombre comp_B, conjunto_CE * conjunto){
    int cant_a = (conjunto->encabezamientos_conjunto)[comp_A].cuenta;
    int cant_b = (conjunto->encabezamientos_conjunto)[comp_B].cuenta;
    if(cant_a > cant_b){// si la componente a tiene mas elementos que la componente b entonces mete el contenido de la b en la a
        fusionar(comp_A, comp_B, conjunto);
    }else{
        fusionar(comp_B, comp_A, conjunto);
    }
}

// dadas dos componentes de un conjunto convierte todos los vertices de la segunda componente en vertices de la primera
void fusionar(tipo_nombre comp_A, tipo_nombre comp_B, conjunto_CE * conjunto){
    tipo_elemento elem = (conjunto->encabezamientos_conjunto)[comp_B].primer_elemento;
        do{
            (conjunto->nombres)[elem].nombre_conjunto = comp_A;
            elem = (conjunto->nombres)[elem].siguiente_elemento;
        }while((conjunto->nombres)[elem].siguiente_elemento != 0);
        // ahora mismo elem contiene el ultimo elemento de la nueva componente
        (conjunto->nombres)[elem].nombre_conjunto = comp_A;
        (conjunto->nombres)[elem].siguiente_elemento = (conjunto->encabezamientos_conjunto)[comp_A].primer_elemento;
        (conjunto->encabezamientos_conjunto)[comp_A].primer_elemento = (conjunto->encabezamientos_conjunto)[comp_B].primer_elemento;
        (conjunto->encabezamientos_conjunto)[comp_A].cuenta += (conjunto->encabezamientos_conjunto)[comp_B].cuenta;
}

tipo_nombre encuentra(int vertice, conjunto_CE * conjunto){
    return ((conjunto->nombres)[vertice].nombre_conjunto);
}

// bueno, hay que seguir con esto xd
// void kruskal(rama ** arbol)