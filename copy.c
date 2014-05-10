/**
COMANDO COPY

copy [OPCIONES] <ruta orixe> <ruta destino>
copy [OPCIONES] <ruta orixe1> <ruta orixe2> ... <ruta orixe> <ruta directorio destino>

OPCIONES:
-R : para copiar directorios recursivamente
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
	char* array[100];
	int i; //espacios ocupados del array
} struct_origenes;

typedef struct {
	int opt_R;
	int opt_t;
	int numBytes;
	int opt_f;
	int opt_v;
} struct_opciones;

struct_opciones opc;
struct_origenes origenes;
char* destino;
long copiedBytes;

/**
** Comprueba si el segundo argumento empieza con el primero.
**/
int startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}


/**
** Convierte un string a int
** Si no es posible, se devuelve un error
**/
int str_to_int(char * arg)
{

	int aux = atoi(arg);
	if (aux == 0)
	{
		printf("ERROR: El valor numBytes especificado para la opción -t no es válido.\n");
		exit(0);
	}

	return aux;
}


void copy_file (char *o, const char *d)
{
	FILE *origen, *destino;
	char buffer[2048];
	int bytesToCopy;

	if ((origen = fopen(o, "rb")) == NULL) {
		printf("No existe el fichero origen\n");
		exit(-1);
	}

	if ((destino = fopen(d,"wb")) == NULL) {
		printf("No se ha creado fichero destino\n");
		exit(-1);
	}
	while(!feof(origen)) {
		bytesToCopy = fread(buffer, 1, sizeof(buffer), origen);
		fwrite(buffer, 1, bytesToCopy, destino);
		copiedBytes = copiedBytes + bytesToCopy;
	}
	fclose(origen);
	fclose(destino);
}



/**
**	Obtiene los argumentos del comando (opciones y nombres orige y destino)
**/
void get_args(int argc, char *argv[])
{
	char * argi;
	char option;
	destino = argv[argc -1];
	printf("destino: %s\n", destino);

	for (int i = 1; i < argc -1; i++) {
		argi = argv[i];
		// OPCIONES
		if (startsWith("-", argi)){
			if (strlen(argi) > 2) 
			{
				printf("ERROR: Opción no válida");
				return;
			}
			option = argi[1];
			printf("option:%c\n", option);
			switch(option)
			{
				case 'R': opc.opt_R = 1; break;
				case 't': opc.opt_t = 1; 
					opc.numBytes = str_to_int(argv[i+1]); 
					printf("\tnumBytes:%d\n", opc.numBytes);
					i++; 
					break;
				case 'f': opc.opt_f = 1; break;
				case 'v': opc.opt_v = 1; break;
				default: printf("ERROR: Opción no reconocida\n"); exit(1);
			}
		}
		// RUTA/S ORIGEN
		else 
		{
			origenes.array[origenes.i] = argi;
			printf("origenes[i]:%s\n", origenes.array[origenes.i]);
			origenes.i++;
		}

	}

}


int main(int argc, char *argv[])
{
	get_args(argc, argv);
}