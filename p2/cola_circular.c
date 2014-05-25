
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
	if((pCola->num) >= (pCola->maxNum))
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

	int fin = pCola->final;
	pCola->items[fin] = item; // Se inserta en la última posición
	pCola->final = next(fin, pCola->maxNum); // Se avanza la última posición
	(pCola->num)++;

	return 1;
}

/**
** Imprime el contenido de la cola
**/
void print(tpCola pCola){
	char* str = (char * ) malloc(50*sizeof(char*));	
	int n = pCola->num;
	int max = pCola->maxNum;
	int numIt;
	
	sprintf (str, "[");
	write (STDOUT_FILENO, str, strlen(str));

	int i = pCola->ppio;
	for(numIt = 0; numIt < n; numIt++){ // Para todos los elementos de la cola
		sprintf (str, " %c", pCola->items[i]);
		write (STDOUT_FILENO, str, strlen(str));
		i = next(i,max);
	}

	sprintf (str, " ]\n");
	write (STDOUT_FILENO, str, strlen(str));
	free(str);
}



int main(){
	tpCola pCola = (tCola *) malloc(sizeof(tCola));
	printf("initialize\n");
	initialize(pCola, 4);

	print(pCola);
	printf("insert:\n");
	insert(pCola,'A');
	insert(pCola,'B');
	insert(pCola,'C');
	print(pCola);
	printf("num=%d\n",pCola->num);
	
	
	free(pCola);
}






