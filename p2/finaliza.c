#include "buffer.h"
#include "semaforo.h"

int main(int argc, char **argv) {

	if (argc != 1) {
		printf("Error: parámetros incorrectos\n");
		exit(0);
	}
	
	int semId = get_sem(); // Obtiene el identificador del semáforo
	del_sem(semId); // Destruye el semáforo
	printf("[padre (pid = %d)] Semáforo destruido!\n", getpid());
	del_buff(); // Destruye el área de memoria compartida
	printf("[padre (pid = %d)] Buffer destruido!\n", getpid());

	return 0;
}
