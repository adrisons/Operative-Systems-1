#include "buffer.h"
#include "semaforo.h"

#define MAXCADENA 100
char mensaje [MAXCADENA];

/**
* Dado un identificador de proceso, devuelve un char entre la 'A' y la 'Z'
**/
char get_item(int id_proc){
	return (char)(id_proc % 26 + 65); //El rango está entre 65 y 90
}


/**
* Añade el char item al buffer
**/
void add_item(int id_proc, char item, tpBuffer pBuffer) {
	
	time_t t = time(NULL);
	pid_t p = getpid();
	sprintf (mensaje, "productor %d (%u): Comienzo anhadir item %c a las %s", id_proc, p, item, ctime(&t));
	write (STDOUT_FILENO, mensaje, strlen(mensaje));

	sleep(random() % 6); // esperar tiempo aleatorio entre 1 y 5 segundos

	insert(pBuffer, item); // añade el item producido al búffer compartido
	print(pBuffer);
	
	sprintf (mensaje, "productor %d (%u): termino de ahadir %c a las %s", id_proc, p, item, ctime(&t));
	write (STDOUT_FILENO,mensaje,strlen(mensaje));
}


/**
* Maneja los semáforos y añade un item al buffer 
* cuando el proceso entra en la sección crítica
**/
void produce(int id_proc, int times, tpBuffer pBuffer, int semId){
	char c;
	int i;
	for (i = 0; i < times; ++i)	{
		c = get_item(id_proc);
		P(semId, SEM_VACIO);
		P(semId, SEM_MUTEX);
		
		add_item(id_proc, c, pBuffer); // Sección crítica

		V(semId, SEM_MUTEX);
		V(semId, SEM_LLENO);
	}
}


/**
* Obtiene los argumentos (opciones, nombres origen y outputFile)
**/
void get_args(int argc, char *argv[], int *num_proc, int *times){
	char * argi;
	char option;
	int i;

	if(argc < 5){
		sprintf(mensaje, "Usage:\n productores -p <Num procesos> -times <Num elementos/proceso>\n");
		write (STDOUT_FILENO, mensaje, strlen(mensaje));
		exit(0);
	}

	for (i = 1; i < argc -1; i++) {
		argi = argv[i];
		// OPCIONES
		if ( argi[0] == '-'){
			if(strcmp(argi, "-times") == 0){
				*times = atoi(argv[i+1]);
				i++; break;
			}

			option = argi[1];
			switch(option){
				case 'p': *num_proc = atoi(argv[i+1]); i++; break;
				default: sprintf(mensaje, "Error: Opción no reconocida \"%s\"\n", argi); 
					write (STDOUT_FILENO, mensaje, strlen(mensaje)); exit(1);
			}
		}
	}
}



int main(int argc, char *argv[]) {
	
	pid_t pid, wpid;
	int status = 0;
	tpBuffer pBuffer;
	int semId; // Identificador del semáforo
	int i, id_proc;
	int np = 0; // Número de procesos a crear.
	int times; // Número de elementos que procesará cada proceso.

	get_args(argc, argv, &np, &times);
	
	sprintf(mensaje,"[padre (pid = %d)] BEGIN \n", getpid());
	write (STDOUT_FILENO, mensaje, strlen(mensaje));

	for (i=0; i<np; i++){
		if ((pid = fork()) == 0){ // hijo
			id_proc = i;
			
			sprintf(mensaje, "[hijo (pid = %d)]\t EMPIEZA\n", getpid());
			write (STDOUT_FILENO, mensaje, strlen(mensaje));

			pBuffer = get_buf();
			semId = get_sem();
			
			produce (id_proc, times, pBuffer, semId);

			leave_buff(pBuffer);
			sprintf(mensaje, "[hijo (pid = %d)]\t ACABA\n", getpid());
			write (STDOUT_FILENO, mensaje, strlen(mensaje));
			return 1;
		}
	}
	
	while ((wpid = wait(&status)) > 0);
	sprintf(mensaje,"[padre (pid = %d)] Sale de la espera\n", getpid());
	write (STDOUT_FILENO, mensaje, strlen(mensaje));

	return 0;
}


