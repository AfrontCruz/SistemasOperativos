#include "head.h"

char path_1[] = "/dev/loop0";
char path_2[] = "/dev/loop1";
char path_3[] = "/dev/loop2";
char path_4[] = "/dev/loop3";
char path_5[] = "/dev/loop4";

int id_mem[5];

char* alloc(char path[], int index){
	char *tmp;
	key_t shamem;

	/*Creamos la llave para la memoría compartida*/
	shamem = ftok( path , 33);

	/*Comprobamos que se haya podido crear la memoría compartida*/
	if( shamem == -1 ){
		//fprintf(stderr,"Hubo un error al iniciar la llave");
		printf("Hubo un error al iniciar la llave\n");
		exit(1);
	}

	/*Obtenemos el ID de la memoría compartida*/
	id_mem[index] = shmget( shamem, 4*sizeof(char), IPC_CREAT | IPC_EXCL | 0777);

	/*Comprobamos que nos haya devuelto el ID*/
	if( id_mem[index] == -1 ){
		id_mem[index] = shmget( shamem, 4*sizeof(char), 0777);
		if( id_mem[index] == -1 )
			perror("Error: imprimiendo error ID memoria...:");
	}

	/*Asignamos la memoría compartida*/
	tmp = (char *)shmat(id_mem[index], NULL, 0);

	if( tmp == NULL){
		perror("No se consiguio la memoria compartida..");
	}

	return tmp;
}

int main(){
	char *mem1 = alloc( path_1, 0 );
	char *mem2 = alloc( path_2, 1 );
	char *mem3 = alloc( path_3, 2 );
	char *mem4 = alloc( path_4, 3 );
	char *mem5 = alloc( path_5, 4 );
	shmdt( mem1 );
	shmdt( mem2 );
	shmdt( mem3 );
	shmdt( mem4 );
	shmdt( mem5 );
	for(int i = 0; i < 5; i++)
		shmctl(id_mem[i], IPC_RMID, NULL);

	printf("La memoria fue liberada...\n");

	return 0;
}