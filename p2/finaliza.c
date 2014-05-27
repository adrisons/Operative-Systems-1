#include "buffer.h"
#include "semaforo.h"

#define MAXCADENA 200
char mensaje [MAXCADENA];

int main(int argc, char **argv) {

	if (argc != 1) {
		sprintf(mensaje, "Usage: finaliza\n");
		write (STDOUT_FILENO, mensaje, strlen(mensaje));
		exit(0);
	}
	
	int semId = get_sem(); // Obtiene el identificador del semáforo
	del_sem(semId); // Destruye el semáforo
	
	sprintf(mensaje, "[padre (pid = %d)] Semáforo destruido!\n", getpid());
	write (STDOUT_FILENO, mensaje, strlen(mensaje));
	
	del_buff(); // Destruye el área de memoria compartida
	
	sprintf(mensaje, "[padre (pid = %d)] Buffer destruido!\n", getpid());
	write (STDOUT_FILENO, mensaje, strlen(mensaje));
	return 0;
}
