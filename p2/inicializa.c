/**
*	Comando 'ipcs' para ver la memoria compartida del sistema y los semáforos
**/

#include "buffer.h"
#include "semaforo.h"

#define MAXCADENA 200
char mensaje [MAXCADENA];

int main(int argc, char **argv) {

	int num_items = 15;

	if((argc == 3 && (strcmp("-items", argv[1]) == 0))){
		int aux = atoi(argv[2]);
		if(aux < MAX_ITEMS && aux > 0)
			num_items = aux;
		else{
			sprintf(mensaje, "Error: el número de items debe estar comprendido entre 0 y %d\n", MAX_ITEMS);
			write (STDOUT_FILENO, mensaje, strlen(mensaje));

			exit(0);
		}
	} else if (argc != 1) {
		sprintf(mensaje, "Usage: inicializa [-items] [num_items]\n");
		write (STDOUT_FILENO, mensaje, strlen(mensaje));

		exit(0);
	}

	tpBuffer pBuffer = get_buf(); // Se crea el área de memoria compartida
	sprintf(mensaje, "[padre (pid = %d)] Buffer creado!\n", getpid());
	write (STDOUT_FILENO, mensaje, strlen(mensaje));
	
	initialize(pBuffer, num_items); // Se inicializa

	int semId = get_sem(); // Se crean los semáforos
	set_sem_value (semId, SEM_VACIO, num_items);
	set_sem_value (semId, SEM_MUTEX, 1);
	set_sem_value (semId, SEM_LLENO, 0);

	sprintf(mensaje, "[padre (pid = %d)] Semáforo creado!\n", getpid());
	write (STDOUT_FILENO, mensaje, strlen(mensaje));
	return 0;
}

/**
int shmget(key_t key, int size, int shmflg); // -1 en caso de error
void * shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr); // 0 en caso de éxito, -1 en caso de error
int shmctl(int shmid, int cmd, struct shmid_ds *buf); // 0 en caso de éxito, -1 en caso de error

proceso padre:
- crear área de memoria compartida
	key_t key = ftok(“/tmp”,’B’);
	int shmid = shmget(key, sizeof(tBuffer), );

proceso hijo:
	tBuffer *pbuffer;
	pbuffer = (tBuffer *) shmat(shmid,0,0);


Al finalizar el hijo:
//desasignar el puntero pbuffer usando shmdt
	int shmdt(pbuffer);

Al finalizar el padre (con finaliza.c):
// destruye el área de memoria compartida
	int shmctl(shmid, IPC_RMID, NULL);


**/