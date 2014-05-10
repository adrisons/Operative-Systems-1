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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
	char* array[10];
	int numFiles; //espacios ocupados del array
} s_input_files;

typedef struct {
	int opt_R;
	int opt_t;
	int numBytes;
	int opt_f;
	int opt_v;
} s_options;

s_options opc;
s_input_files inputFiles;
char* outputFile;
int copiedBytes;



/**
** Comprueba si el segundo argumento empieza con el primero.
**/
int starts_with(const char *pre, const char *str)
{
    size_t lenPre = strlen(pre),
           lenStr = strlen(str);
    return lenStr < lenPre ? 0 : strncmp(pre, str, lenPre) == 0;
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
		printf("Error: El valor numBytes especificado para la opción -t no es válido.\n");
		exit(0);
	}

	return aux;
}

/**
** Copia un fichero a otro
** Si no tiene permisos, se devuelve un error
**/
void copy_file_to_file (char *o, const char *d)
{
	FILE *inputFile, *outputFile;
	char buffer[2048];
	int bytesToCopy;

	if ((inputFile = fopen(o, "rb")) == NULL) {
		printf("Error: No existe el fichero inputFile\n");
		exit(-1);
	}

	if ((outputFile = fopen(d,"wb")) == NULL) {
		printf("Error: No se ha creado fichero outputFile\n");
		exit(-1);
	}
	while(!feof(inputFile)) {
		bytesToCopy = fread(buffer, 1, sizeof(buffer), inputFile);
		fwrite(buffer, 1, bytesToCopy, outputFile);
		copiedBytes = copiedBytes + bytesToCopy;
	}
	fclose(inputFile);
	fclose(outputFile);
}



/**
**	Obtiene los argumentos (opciones, nombres origen y outputFile)
**/
void get_args(int argc, char *argv[])
{
	char * argi;
	char option;
	outputFile = argv[argc -1];
	printf("outputFile:%s\n", outputFile);

	for (int i = 1; i < argc -1; i++) {
		argi = argv[i];
		// OPCIONES
		if (starts_with("-", argi)){
			if (strlen(argi) > 2) 
			{
				printf("Error: Opción no válida");
				exit(1);
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
				default: printf("Error: Opción no reconocida \"%c\"\n", option); exit(1);
			}
		}
		// RUTAS ORIGEN
		else 
		{
			inputFiles.array[inputFiles.numFiles] = argi;
			printf("inputFiles[i]:%s\n", inputFiles.array[inputFiles.numFiles]);
			inputFiles.numFiles++;
		}

	}

}


/**
** Obtiene el tipo de un path.
** Devuelve:
**	0 - error
**	1 - directorio
**	2 - fichero
**	3 - link
**/
int get_path_type(const char* target)
{
	struct stat statbuf;
	if(lstat(target, &statbuf) != 0)
	{
		printf("Error: Path incorrecto.\n");
		exit(3);
	}
	if(S_ISDIR(statbuf.st_mode))
		// Es un directorio
		return 1;
	else if(S_ISREG(statbuf.st_mode))
		// Es un fichero
		return 2;
	else if(S_ISLNK(statbuf.st_mode))
		// Es un link
		return 3;
	else // Error
	{
		printf("Error: Tipo de archivo incorecto.\n");
		exit(8);
		return 0;
	} 
}

/**
**
** No permite copiar un archivo sobre si mismo
** Si el destino es un fichero, el origen sólo puede ser un fichero.
**
** Si hay varios paths origen
** 		los origenes deben ser ficheros o links
**		el destino debe ser un directorio
** 		
**/
void check_paths()
{
	int dest_type = get_path_type(outputFile);
	int num_files_in = inputFiles.numFiles;

	if(num_files_in > 1){
		// El path destino tiene que ser un directorio
		if(dest_type != 1){
			printf("Error: Si se indican varios paths origen, el path destino debe ser un directorio.\n");
			exit(9);
		}

		for (int i = 0; i < num_files_in; i++)
		{
			int orig_type = get_path_type(inputFiles.array[i]);
			// Los paths origen tienen que ser ficheros o links
			if(orig_type != 2 && orig_type != 3){
				printf("Error: Si se indican varios paths origen, estos deben de ser ficheros o enlaces simbólicos.\n");
				exit(7);
			}
		}
	}
	else if(num_files_in == 1){
		char* inputFile = inputFiles.array[0];
		int orig_type = get_path_type(inputFiles.array[0]);
		// Si el path origen es un directorio debe estar especificada la opción -R
		if(orig_type == 1){
			if(dest_type != 1){
				printf("Error: Para copiar directorios, el destino debe ser un directorio.\n");
				exit(10);
			}
			if(!opc.opt_R){
				printf("Error: Para copiar directorios debe estar activada la opción -R.\n");
				exit(11);
			}
		}
		// Por defecto no se permite copiar un archivo sobre si mismo
		else if ( (orig_type == 2 || orig_type == 3) && (!opc.opt_f) && (strcmp(inputFile, outputFile) == 0) ) {
			printf("Error: Para sobreescribir archivos debe estar activada la opción -f.\n"); 
			exit(4);
		}
		
	}

	if(dest_type != 1 && dest_type != 2)
	{
		printf("Error: El path destino debe ser un fichero o un directorio\n");
		exit(6);
	}

}


void copy()
{
	int dest_type = get_path_type(outputFile);
	int num_files_in = inputFiles.numFiles;

	check_paths();

	// PATHS ORIGEN
	if(num_files_in > 1){
		printf("Copiar varios ficheros a un directorio\n");
		// TODO: copy_many_files_to_dir
	}
	else if(num_files_in == 1){
		int orig_type = get_path_type(inputFiles.array[0]);
		//char* input = inputFiles.array[0];

		if(orig_type == 1){ // Si el origen es un directorio
			printf("Copiar recursivamente un directorio en otro\n");
			// TODO: copy_dir_rec(input, outputFile);
		}
		else{
			if(dest_type == 1){
				printf("Copiar un fichero en un directorio\n");
				//copy_file_to_dir(input, outputFile);
			}
			else if (dest_type == 2){
				printf("Copiar un fichero en un fichero\n");
				//copy_file_to_file(input, outputFile);
			}
		}
	}

}


int main(int argc, char *argv[])
{
	get_args(argc, argv);
	copy();
	//copy_file_to_file(inputFiles.array[0], outputFile);

}