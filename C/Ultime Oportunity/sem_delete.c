#include "head.h"

int createSystemV(int no_sem){
	int sem_id;
	key_t key;

	/* Creamos la llave para el semaforo */
	key = ftok( "/bin/ls", 33 );
	if( key == -1 ){
		perror("Error al abrir la llave");
		exit(-1);
	}
	/* Creamos el semáforo SystemV*/
	sem_id = semget( key, no_sem, IPC_CREAT | IPC_EXCL | 0777 );

	/*Verificamos que el semaforo se haya creado*/
	if( sem_id == -1 ){
		sem_id = semget( key, no_sem, 0777 );
		if( sem_id == -1 ){
			/*Cachamos el error de errno SystemV*/
			//int error = errno;
			//fprintf(stderr, "Valor de errno para Semaforo SystemV: %d\n", error);
			perror("Error: imprimiendo error SystemV...:");
			//fprintf(stderr, "Error SystemV:\n %s\n", stderror( error ) );
			exit(-1);
		}
	}
	//printf("El semaforo se creo sin problemas!!!!\n");

	return sem_id;
}

int main(){
	int id = createSystemV(7);
	for( int j = 0; j < 7; j++ ){
		printf("Eliminando sem %i ...\n", j + 1);
		semctl( id, j ,IPC_RMID, 0);
	}
}

