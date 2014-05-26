#include "buffer.h"
//#include "semaforo.h"

int main(int argc, char **argv) {

	tpBuffer pBuffer = get_buf();
	initialize(pBuffer, MAX_ITEMS);

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