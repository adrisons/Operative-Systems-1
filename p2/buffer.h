#include "cola_circular.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

typedef struct cola tBuffer;
typedef tBuffer * tpBuffer;

/** Funciones de cola_circular:
	void initialize(tpCola pCola, int tam); // Inicializa la cola con maxNum = tam
	itemType get_head(tpCola);
	int insert(tpCola, itemType);
	void print(tpCola);
**/

int get_buff_id();
tpBuffer get_buf();
void buff_status();
void leave_buff(tpBuffer pBuffer);
void del_buff();