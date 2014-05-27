#include "semaforo.h"

/**
* Obtiene el identificador del área de memoria compartida.
* Si no existe, se crea y se obtiene el identificador.
**/
int get_sem(){
	key_t key = ftok(PATH_SHSEM, SEM_ID);
	if((int)key == -1){
		printf("Error [ftok]: el path no existe o no puede ser accedido por el proceso.\n");
		exit(0);
	}
	int semId = semget(key, NUM_SEM, IPC_CREAT | 0666); //IPC_CREAT: Create the shared memory segment if it does not exist.
	if(semId == -1){
		printf("Error [semget]: al obtener el identificador del semáforo.\n");
		exit(0);
	}

	return semId;
}


int get_sem_value (int semId, int semnum){
	int i = semctl(semId, semnum, GETVAL);
	if (i == -1) {
		printf("Error [semctl]: al obtener el valor del semáforo.\n");
		exit(0);
	}
	return i;
}

void set_sem_value (int semid, int semnum, int value){	
	int i = semctl (semid, semnum, SETVAL, value);
	if (i == -1) {
		printf("Error [semctl]: al actualizar el valor del semáforo.\n");
		exit(0);
	}
}


void sem_op(int semId, int semnum, int op){

	struct sembuf op_buf[1];
	op_buf[0].sem_num = semnum;
	op_buf[0].sem_op = op; /* Realiza la operación indicada sobre semval */
	op_buf[0].sem_flg = 0; /* Permite que se produzca un wait */
	
	if (semop(semId, op_buf, 1) == -1) {
		printf("Error [semop]: al modificar el semáforo %d [pid=%d].\n", semnum, getpid());
		exit(0);
	}
}

/**
* Decrementa el semval en 1
**/
void P(int semId, int semnum){
	sem_op(semId, semnum, -1);
}

/**
* Incrementa el semval en 1
**/
void V(int semId, int semnum){
	sem_op(semId, semnum, 1);
}

/**
* Borra el semáforo semId
**/
void del_sem(int semId){
	int err = semctl(semId, 1, IPC_RMID);
	if(err == -1){
		printf("Error [semctl]: al eliminar el semáforo.\n");
		exit(0);	
	}
}


/************************************************************************
int main(int argc, char const *argv[])
{
	int semId = get_sem();

	int i1 = get_sem_value(semId,SEM_VACIO);
	int i2 = get_sem_value(semId,SEM_LLENO);
	int i3 = get_sem_value(semId,SEM_MUTEX);
	printf(" SEM_VACIO=%d\n SEM_LLENO=%d \n SEM_MUTEX=%d\n",i1,i2,i3);


	set_sem_value(semId,SEM_VACIO,1);
	set_sem_value(semId,SEM_LLENO,2);
	set_sem_value(semId,SEM_MUTEX,3);

printf("------------------------------------\n");	
	i1 = get_sem_value(semId,SEM_VACIO);
	i2 = get_sem_value(semId,SEM_LLENO);
	i3 = get_sem_value(semId,SEM_MUTEX);
	printf(" SEM_VACIO=%d\n SEM_LLENO=%d \n SEM_MUTEX=%d\n",i1,i2,i3);
printf("------------------------------------\n");	

	V(semId,SEM_VACIO);
	P(semId,SEM_LLENO);
	P(semId,SEM_LLENO);
	P(semId,SEM_MUTEX);

	i1 = get_sem_value(semId,SEM_VACIO);
	i2 = get_sem_value(semId,SEM_LLENO);
	i3 = get_sem_value(semId,SEM_MUTEX);
	printf(" SEM_VACIO=%d\n SEM_LLENO=%d \n SEM_MUTEX=%d\n",i1,i2,i3);
	
	del_sem(semId);
	return 0;
}
************************************************************************/