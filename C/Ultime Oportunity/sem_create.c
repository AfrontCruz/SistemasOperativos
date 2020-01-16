#include "head.h"

/*
***	Esta función crea el semáforo SystemV
*/
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
			perror("Error: imprimiendo error SystemV...:");
			exit(-1);
		}
	}
	//printf("El semaforo se creo sin problemas!!!!\n");

	return sem_id;
}

int main(){
	int proceso, estado;
	char aux;

	int id_r = createSystemV(7);
	printf("Padre: El id del sem es: %i\n", id_r );
	semctl( id_r, 0, SETVAL, 5 );
	semctl( id_r, 1, SETVAL, 0 );
	semctl( id_r, 2, SETVAL, 1 );
	semctl( id_r, 3, SETVAL, 1 );
	semctl( id_r, 4, SETVAL, 1 );
	semctl( id_r, 5, SETVAL, 1 );
	semctl( id_r, 6, SETVAL, 1 );
	semctl( id_r, 7, SETVAL, 1 );

	return 0;
}