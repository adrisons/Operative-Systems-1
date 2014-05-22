/**
COMANDO COPY

copy [OPCIONES] <ruta origen> <ruta destino>
copy [OPCIONES] <ruta origen1> <ruta origen2> ... <ruta origen> <ruta directorio destino>

OPCIONES:
-R : para copiar directorios recursivamente
-t num_Bytes : limita el tamaño máximo por archivo que se puede copiar.
-f : para sobrescribir archivos ya existentes con la ruta destino.
-v : activa la visualización del proceso de copia.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <libgen.h>

#define MAX_ELEMENTOS 15

typedef struct {
	char* array[MAX_ELEMENTOS];
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
int copiedBytes = 0;



/**
** Comprueba si el segundo argumento empieza con el primero.
**/
int starts_with(char *pre, char *str){
    size_t lenPre = strlen(pre),
           lenStr = strlen(str);
    return lenStr < lenPre ? 0 : strncmp(pre, str, lenPre) == 0;
}

/**
** Comprueba si el segundo argumento acaba con el primero.
**/
int ends_with(char *suc, char *str){
	size_t lenSuc = strlen(suc),
			lenStr = strlen(str);

	return lenStr < lenSuc ? 0 : strcmp(suc, &str[lenStr-lenSuc]) == 0;
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



void copy_perm(char *input, char *output){

	struct stat info;
	if (lstat(input, &info) == -1){
		printf ("Error: No se han podido coger los permisos de %s\n", input);
		exit(0);
	}
	chmod(output, info.st_mode);

}



/**
** Obtiene el tipo de un path.
** Devuelve:
**	0 - error
**	1 - directorio
**	2 - fichero
**	3 - link
**/
int get_path_type(char* target){
	struct stat statbuf;
	if(lstat(target, &statbuf) == -1){
		int errsv = errno;
		if (errsv == ENOENT){
			//El fichero no existe -> se va a crear
			return 2;
		}
		else{
			printf("Error: Path incorrecto %s.\n",target);
			exit(0);
		}
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
		printf("Error: Tipo de archivo incorecto %s.\n",target);
		return 0;
	} 
}



/**
** 0 - Todo correcto.
** tamaño archivo - archivo demasiado grande.
**/
int check_opt_t(char * o){

	if(opc.opt_t){ //Si no está activada la opción -t no se comprueba
		struct stat info;
		
		if( (stat(o, &info)) != 0){
			printf("Error: Al usar stat sobre el fichero %s.\n", o);
			exit(0);
		}
		int tam_o = info.st_size;
		if (tam_o > opc.numBytes){
			printf("Error: opción -t activada, tam(%s)=%d > %d.\n",o,tam_o, opc.numBytes);
			return tam_o;
		}
	}
	return 0;
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
void check_paths(char* input_path, char * output_path){

	if(strcmp(input_path,output_path) == 0){
		printf("Error: Las rutas origen y destino no pueden ser la misma.\n");
		exit(0);
	}

	int orig_type = get_path_type(input_path);
	
	if((orig_type == 1) && (!opc.opt_R)){// Si el path origen es un directorio debe estar especificada la opción -R
			printf("Error: Para copiar directorios debe estar activada la opción -R.\n");
			exit(0);
	}
	// Si el fichero ya existe tiene que estar indicada la opción -f para sobrescribir
	else if ( (orig_type == 2 || orig_type == 3) && (!opc.opt_f) ) {
		FILE * file = fopen(output_path, "r");
		if( file != NULL ){
			printf("Error: Ya existe %s. Para sobrescribir archivos debe estar la opción -f activada.\n", output_path);
			fclose(file);
			exit(0);
		}			
	}
}


/**
** Abre un fichero y devuelve un descriptor al mismo en caso de éxito, o un error.
**/
FILE * open_file(char * path, char * options){
	FILE * f = fopen(path, options);
	if (f == NULL) {
		printf("Error: No hay acceso al fichero %s.\n", path);
		exit(0);
	}
	return f;
}

/**
** Copia un fichero a otro
** Si no tiene permisos, se devuelve un error
**/
void copy_file_to_file (char * input_path, char * output_path){
	FILE *inputFile, *outputFile;
	char buffer[2048];
	int bytesToCopy;
	char * input_real_path;

	check_paths(input_path, output_path);

	int tam_arch = check_opt_t(input_path);
	if(tam_arch){
		if(opc.opt_v) printf("Archivo demasiado grande [%s] %d bytes\n", input_path, tam_arch);
		return; // Si no cumple la opt_t se pasa al siguiente fichero
	}

	if(opc.opt_v) printf("%s\t --> ", input_path);
	
	int path_type = get_path_type(input_path);
	input_real_path = realpath (input_path, NULL); // Se obtiene al fichero al que apunta el link
	if ((path_type == 3) && (opc.opt_R)){// Si la entrada es un link // Si está activada la opción -R se copia el enlace
		symlink (input_real_path, output_path);
	}
	else {
		if(path_type == 3) // -R no está activada, así que se copia el fichero original
			input_path = input_real_path;
		
		inputFile =  open_file(input_path,"rb");
		outputFile = open_file(output_path,"wb");
		
		while(!feof(inputFile)) {
			bytesToCopy = fread(buffer, 1, sizeof(buffer), inputFile);
			fwrite(buffer, 1, bytesToCopy, outputFile);
			copiedBytes += bytesToCopy;
		}
		fclose(inputFile);
		fclose(outputFile);
	}

	copy_perm(input_path, output_path);
	if(opc.opt_v) printf("%s\n", output_path);

}



void copy_dir_to_dir(char * input, char * output){

	DIR * dir_origen;
	struct dirent * inside_origen;
	
	char * path_input = (char *) malloc (1000 *sizeof (char));
	char * path_input_b = (char *) malloc (1000 *sizeof (char));
	char * path_output = (char *) malloc (1000 *sizeof (char));
	char * path_output_b = (char *) malloc (1000 *sizeof (char));

	check_paths(input, output);

	if ((dir_origen = opendir (input)) == NULL) {
		printf("Error: No se ha podido abrir %s\n", input);
		exit(0);
	}

		strcpy (path_input_b, input);
		if(opc.opt_v) printf("%s\t --> ", path_input_b);
		
		sprintf(path_output_b,"%s/%s", path_output, basename(input));

		mkdir(path_output_b, 0777);
		copy_perm(input, path_output_b);
		if(opc.opt_v) printf("%s\n", path_output_b);

	while ((inside_origen = readdir (dir_origen)) != NULL) {
		char * origen_file = inside_origen -> d_name;

  		if (origen_file[0] == '.') {continue;} // Se ignoran los archivos ocultos como . o ..

		sprintf(path_output,"%s/%s", path_output_b, origen_file);
		sprintf(path_input,"%s/%s", path_input_b, origen_file);


		int file_type = get_path_type(path_input);
		switch(file_type)
		{
			case 0: break; 
			case 1:	copy_dir_to_dir(path_input, path_output_b); break;
			case 2:
			case 3: copy_file_to_file(path_input, path_output); break;
			default: break;
		}
  	}
  	free(path_input);
	free(path_input_b);
	free(path_output);
  	free(path_output_b);
	closedir(dir_origen);
}



void copy(char* array_input[MAX_ELEMENTOS], int num_files_in, char * output)
{
	int dest_type = get_path_type(output);

	for(int i=0; i < num_files_in; i++){
		int orig_type = get_path_type(inputFiles.array[i]);
		char * input = inputFiles.array[i];

		if(orig_type == 1){ // Si el origen es un directorio
			copy_dir_to_dir(input, output);
		}
		else { // El origen es un fichero
			if(dest_type == 1){ // El destino es un directorio
				char * path_output = (char *) malloc (1000 *sizeof (char));

				sprintf(path_output,"%s/%s", output, basename(input));
				copy_file_to_file(input, path_output);
				free(path_output);
			}
			else if (dest_type == 2){ // El destino es un fichero
				copy_file_to_file(input, output);
			}
		}
	}
}



/**
**	Obtiene los argumentos (opciones, nombres origen y outputFile)
**/
void get_args(int argc, char *argv[]){
	char * argi;
	char option;
	outputFile = argv[argc -1];

	for (int i = 1; i < argc -1; i++) {
		argi = argv[i];
		// OPCIONES
		if (starts_with("-", argi)){
			if (strlen(argi) > 2) {
				printf("Error: Opción no válida");
				exit(0);
			}
			option = argi[1];
			switch(option){
				case 'R': opc.opt_R = 1; break;
				case 't': opc.opt_t = 1; 
					opc.numBytes = str_to_int(argv[i+1]);
					i++; 
					break;
				case 'f': opc.opt_f = 1; break;
				case 'v': opc.opt_v = 1; break;
				default: printf("Error: Opción no reconocida \"-%c\"\n", option); exit(0);
			}
		}
		// RUTAS ORIGEN
		else {

			inputFiles.array[inputFiles.numFiles] = argi;
			inputFiles.numFiles++;
		}
	}
	if(inputFiles.numFiles == 0){
		printf("Error: Es necesario especificar un origen a copiar.\n");
		exit(0);
	}
}


int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("copy [OPCIONES] <ruta origen> <ruta destino>\n");
		printf("copy [OPCIONES] <ruta origen> <ruta origen2> ... <ruta origen> <ruta directorio destino>\n");
		printf("OPCIONES:\n");
		printf("\t-R : para copiar directorios recursivamente.\n");
		printf("\t-t num_Bytes : limita el tamaño máximo por archivo que se puede copiar.\n");
		printf("\t-f : para sobrescribir archivos ya existentes con la ruta destino.\n");
		printf("\t-v : activa la visualización del proceso de copia.\n");
		exit(0);
	}

	get_args(argc, argv);
	copy(&inputFiles.array[0], inputFiles.numFiles, outputFile);
	printf("Bytes copiados: %d\n", copiedBytes);


}