#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int tipo_nombre;
typedef int tipo_elemento;
typedef int vertice;

#define VERTICES 5

typedef struct _ARISTA{
vertice u;
vertice v;
int costo;
} arista;

typedef struct _RAMA{
struct _ARISTA a;
struct _RAMA * sig;
} rama;

typedef struct _ENCABEZADO{
    int cuenta;
    int primer_elemento;
} encabezado;

typedef struct _NOMBRE{
    tipo_nombre nombre_conjunto;
    int siguiente_elemento;
} nombre;

typedef struct _CONJUNTO_CE{
    nombre nombres [VERTICES];
    encabezado encabezamientos_conjunto [VERTICES];
} conjunto_CE;

void inicial(tipo_nombre, tipo_elemento, conjunto_CE *);
void combina(tipo_nombre, tipo_nombre, conjunto_CE *);
tipo_nombre encuentra (int, conjunto_CE *);
void kruskal (rama *);

void inserta (int, int, int, rama **);
arista * sacar_min (rama **);
void lista (rama *);

int main ()
{

int M_Costos [VERTICES][VERTICES];
rama * arbol;

for (int i = 0; i <= VERTICES -1; i++)
    for (int j = i + 1; j <= VERTICES -1; j++)
        {
 printf("Ingrese costo de lado entre vertices %d y %d : ", i, j) ;

 scanf("%d", &M_Costos [i][j]);
 }

for(int i = 0; i <= VERTICES -1; i ++) // la mitad inf. de diagonal de matriz
    for(int j = i + 1; j <= VERTICES -1; j++)
        if(M_Costos [i][j] != 0)
inserta (i, j, M_Costos[i][j], &arbol); //inserto en cola prior .
kruskal(arbol);
return 0;
}
