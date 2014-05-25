
#include "cola_circular.h"


void initialize(tpCola pCola, int tam){
	if(tam > MAX_ITEMS){
		printf("El tamaño de la cola (%d) no puede superar el máximo (%d)\n", tam, MAX_ITEMS);
		exit(0);
	}
	pCola->maxNum = tam;
	pCola->num = 0;
	pCola->ppio = 0;
	pCola->final = 0;
}


int isEmpty(tpCola pCola){
	if((pCola->num) == 0)
		return 1;
	return 0;
}

int isFull(tpCola pCola){
	if((pCola->num) >= (pCola->maxNum -1))
		return 1;
	return 0;
}

int next (int pos, int max){
	return ((pos + 1) % max);
}


/** 
** Obtiene el primer elemento de la cola y lo elimina.
** Si la cola está vacía devuelve error.
**/
itemType get_head(tpCola pCola){
	if(isEmpty(pCola)){
		printf("Error: la cola está vacía.\n");
		exit(0);
	}

	itemType res;
	int ini = pCola->ppio;
	res = pCola->items[ini]; // Se obtiene el primer elemento
	pCola->ppio = next(ini, pCola->maxNum); // Se avanza la primera posición
	(pCola->num)--;
	return res;

}


/**
** Inserta itemType al final de la cola
**/
int insert(tpCola pCola, itemType item){
	if(isFull(pCola)) // Si está llena no se puede insertar
		return 0;

	pCola->final = next(pCola->final, pCola->maxNum); // Se avanza la última posición
	pCola->items[pCola->final] = item; // Se inserta en la última posición
	(pCola->num)++;

	return 1;
}

/**
** Imprime el contenido de la cola
**/
void print(tpCola pCola){
	char * str;
	int i = pCola->ppio;
	int m = pCola->maxNum;
	int f = pCola->final;
	str = "[";
	write (STDOUT_FILENO, str, strlen(str));
	
	while((i = next(i, m)) != f){
		sprintf (str, "%c\n", pCola->items[i]);
		write (STDOUT_FILENO, str, strlen(str));
	}
	str = "]\n";
	write (STDOUT_FILENO, str, strlen(str));

}



int main(){
	tpCola pCola;
	initialize(pCola, 4);
	print(pCola);
}






