#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_SEM 3
#define SEM_VACIO 0
#define SEM_LLENO 1
#define SEM_MUTEX 2
#define PATH_SHSEM "/tmp"
#define SEM_ID 's'


//initialize() // semctl SETALL
//print() // semctl GETALL


int get_sem(); // Obtiene el identificador del área de memoria compartida. Si no existe, se crea y se obtiene el identificador.
int get_sem_value (int semId, int semnum);
void set_sem_value (int semid, int semnum, int value);

void P(int semId, int semnum); // Decrementa el semval en 1
void V(int semId, int semnum); // Incrementa el semval en 1
void del_sem(int semId);

