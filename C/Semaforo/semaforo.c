#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){
	int *valor = NULL;
	sem_t semaforo[2];

	//Creamos la llave
	key_t key;
	int id;

	key_t key_sem;
	int id_sem;

	//Inicializamos la llave
	key = ftok( "/home/afront/Documentos/SO/C", 33);
	if( key == -1 ){
		perror("Hubo un error al iniciar la llave");
		exit(1);
	}

	//Obtenemos el id de la MC
	id = shmget( key, sizeof(int)*1, IPC_CREAT ); //0666 es el macro IPC_CREAT
	if( id == -1 ){
		perror("Hubo un error al obtener el id");
		exit(1);
	}

	//Apuntamos nuestra variable a la zona compartida
	valor = (int *)shmat( id, NULL, 0 );
	if( valor == NULL){
		perror("No se consiguio la memoria compartida..");
		exit(1);
	}

	//ZONA SEM
	key_sem = ftok("/bin/ls", 33);
	if( key_sem == -1 ){
		perror("Hubo un error al iniciar la llave");
		exit(1);
	}

	//Obtenemos el id de la MC
	id_sem = shmget( key_sem, 2*sizeof(sem_t), IPC_CREAT ); //0666 es el macro IPC_CREAT
	if( id_sem == -1 ){
		perror("Hubo un error al obtener el id");
		exit(1);
	}

	//Apuntamos nuestra variable a la zona compartida
	sem = (sem_t)shmat( id, NULL, 0 );
	if( valor == NULL){
		perror("No se consiguio la memoria compartida..");
		exit(1);
	}

	//Crea semaforo
	//-sem_t semaforo;

	//inicializar semaforo
	if( sem_init( &semaforo, 1, 1) == -1 ){
		exit(1);
	}

	//Crea el proceso
	pid_t pid; 

	//Inicializar el proceso
	pid = fork();

	switch( pid ){
		case -1:	perror("Hubo un error\n");
					break;
		case 0:		//El hijo
					for(int i = 0; i < 10; i++){
						sem_wait( &semaforo ); //Cierra el semaforo
						valor[0]--;
						printf("%i.Consumidor: %i\n", i + 1, valor[0]);
						sem_post( &semaforo );
					}
					break;
		default:	//El padre
					for(int i = 0; i < 10; i++){
						sem_wait( &semaforo ); //Cierra el semaforo
						valor[0]++;
						printf("%i.Productor: %i\n", i + 1, valor[0]);
						sem_post( &semaforo );
					}
					wait(0); //Espera a que se muera el hijo
					sem_destroy( &semaforo ); //Destruye el semaforo
					printf("Fin\n");
					break;
	}

	//Liberamos la memoria compartida
	shmdt((char  *) valor );
	shmctl( id, IPC_RMID, (struct shmid_ds *)NULL );
	shmdt((char  *) sem );
	shmctl( id_sem, IPC_RMID, (struct shmid_ds *)NULL );
	return 0;
}
