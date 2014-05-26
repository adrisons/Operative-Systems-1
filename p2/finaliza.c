#include "buffer.h"
//#include "semaforo.h"

int main(int argc, char **argv) {

	if (argc != 1) {
		printf("Error: parámetros incorrectos\n");
		exit(0);
	}

	del_buff(); // Destruye el área de memoria compartida

	return 0;
}
