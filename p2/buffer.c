#include "buffer.h"


#define PATH_SHMEM "/tmp" // Path en donde se creará el área de memoria compartida
#define USR_ID 'B' // Identificador de usuario para obtener el área de memoria compartida

/**
* Obtiene el identificador del área de memoria compartida.
* Si no existe, se crea y se obtiene el identificador.
**/
int get_buff_id(){
	key_t key = ftok(PATH_SHMEM, USR_ID);
	if((int)key == -1){
		printf("Error [ftok]: el path no existe o no puede ser accedido por el proceso.\n");
		exit(0);
	}
	int shmid = shmget(key, sizeof(tBuffer), IPC_CREAT | 0666); //IPC_CREAT: Create the shared memory segment if it does not exist.
	if(shmid == -1){
		printf("Error [shmget]: al obtener el identificador de la memoria compartida.\n");
		exit(0);
	}

	return shmid;
}

/**
* Devuelve un puntero a la memoria compartida
**/
tpBuffer get_buf(){
	int shmid = get_buff_id();
	tpBuffer pBuffer;
	pBuffer = (tpBuffer) shmat(shmid, 0, 0);
	if(pBuffer == (tpBuffer)-1){
		printf("Error shmat\n");
		exit(0);
	}
	return pBuffer;
}


/**
* Muestra información acerca de la memoria compartida
**/
void buff_status(){
	int shmid = get_buff_id();
	struct shmid_ds *buf = (void*) malloc(sizeof(struct shmid_ds));
	if(shmctl(shmid, IPC_STAT, buf) == -1){
		printf("Error al mostrar estado.[errno=%d]\n", errno);
		
	}else{
		/* Print the current status. */
		printf("\nCurrent status:\n");
		printf("\tshm_perm.uid = %d\n", buf->shm_perm.uid);
		printf("\tshm_perm.gid = %d\n", buf->shm_perm.gid);
		printf("\tshm_perm.cuid = %d\n", buf->shm_perm.cuid);
		printf("\tshm_perm.cgid = %d\n", buf->shm_perm.cgid);
		printf("\tshm_perm.mode = %#o\n", buf->shm_perm.mode);
		printf("\tshm_perm.key = %#x\n", buf->shm_perm._key);
		printf("\tshm_segsz = %d\n",(int) buf->shm_segsz);
		printf("\tshm_lpid = %d\n", buf->shm_lpid);
		printf("\tshm_cpid = %d\n", buf->shm_cpid);
		printf("\tshm_nattch = %d\n", buf->shm_nattch);
		printf("\tshm_atime = %s", buf->shm_atime ? ctime(&buf->shm_atime) : "Not Set\n");
		printf("\tshm_dtime = %s", buf->shm_dtime ? ctime(&buf->shm_dtime) : "Not Set\n");
		printf("\tshm_ctime = %s", ctime(&buf->shm_ctime));
	}
	free(buf);

}


/**
* Se desasigna el puntero al buffer
**/
void leave_buff(tpBuffer pBuffer){
	if( shmdt(pBuffer) == -1){
		printf("Error: al desasignar el puntero al buffer.\n");
		exit(0);
	}
}


/**
* Se destruye el área de memoria compartida
**/
void del_buff(){
	int shmid = get_buff_id();
	if( shmctl(shmid, IPC_RMID, NULL) == -1){
		printf("Error: al borrar el buffer.\n");
		exit(0);
	}

}



int main(){

	get_buf();
	printf("Buffer creado!\n");

	buff_status();

	del_buff();
	printf("Buffer destruido!\n");

}

