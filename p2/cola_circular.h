#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_ITEMS 100


typedef char itemType; // Va a ser una cola de chars. Se define item para una implementación más genérica

typedef struct {
	itemType items[MAX_ITEMS]; // Vector de longitud indicada en “inicializa.c”
	int maxNum; // Maximo numero de items que caben en la cola. maxNum <= MAX_ITEMS
	int num; // Numero de elementos actualmente almacenados en la cola.
	int ppio; // Primer elemento ocupado de la cola.
	int final; // Ultimo elemento ocupado de la cola.
} tCola;

typedef tCola *tpCola;

int isEmpty(tpCola);
itemType get_head(tpCola); // Obtiene el primero de la cola
int insert(tpCola, itemType); // Inserta item al final de la cola
void print(tpCola); // Imprime el contenido de la cola
