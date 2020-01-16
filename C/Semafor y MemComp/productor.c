#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define PRODUCTOR 0
#define CONSUMIDOR 1

int id_mem;

/*
***	Esta función crea la memoría compartida
*/
int* alloc(){
	int *tmp;
	key_t shamem;

	/*Creamos la llave para la memoría compartida*/
	shamem = ftok("/dev/null", 33);

	/*Comprobamos que se haya podido crear la memoría compartida*/
	if( shamem == -1 ){
		//fprintf(stderr,"Hubo un error al iniciar la llave");
		printf("Hubo error al iniciar la llave\n");
		exit(1);
	}

	/*Obtenemos el ID de la memoría compartida*/
	id_mem = shmget( shamem, sizeof(int), IPC_CREAT | IPC_EXCL | 0777);

	/*Comprobamos que nos haya devuelto el ID*/
	if( id_mem == -1 ){
		id_mem = shmget( shamem, sizeof(int), 0777);

		if( id_mem == -1 ){
			/**/
			//int error = errno;
			//fprintf(stderr, "Valor de errno en el ID memoria: %d\n", error);
			perror("Error: imprimiendo error ID memoria...:");
			//fprintf(stderr, "Error ID memoria:\n %s\n", stderror( error ) );
			exit(-1);

		}
	}

	/*Asignamos la memoría compartida*/
	tmp = (int *)shmat(id_mem, NULL, 0);

	if( tmp == NULL){
		perror("No se consiguio la memoria compartida..");
			
	}

	return tmp;
}

/*
***	Esta función crea el semáforo SystemV
*/
int createSystemV(){
	int sem_id;
	key_t key;

	/* Creamos la llave para el semaforo */
	key = ftok( "/bin/ls", 33 );
	if( key == -1 ){
		perror("Error al abrir la llave");
		exit(-1);
	}
	/* Creamos el semáforo SystemV*/
	sem_id = semget( key, 2, IPC_CREAT | IPC_EXCL | 0777 );

	/*Verificamos que el semaforo se haya creado*/
	if( sem_id == -1 ){
		sem_id = semget( key, 2, 0777 );
		if( sem_id == -1 ){
			/*Cachamos el error de errno SystemV*/
			//int error = errno;
			//fprintf(stderr, "Valor de errno para Semaforo SystemV: %d\n", error);
			perror("Error: imprimiendo error SystemV...:");
			//fprintf(stderr, "Error SystemV:\n %s\n", stderror( error ) );
			exit(-1);
		}
	}
	printf("El semaforo se creo sin problemas!!!!\n");

	return sem_id;
}

void semWait(int sem, int n){
	struct sembuf sb = { n, -1, SEM_UNDO };
	if( semop(sem, &sb, 1) != -1 ){
		printf("Semaforo productor bloqueado\n");
	}else{
		printf("Hubo un problema al bloquear el semaforo productor\n");
	}
}

void semSignal(int sem, int n){
	struct sembuf sb = { n, 1, SEM_UNDO };
	if( semop(sem, &sb, 1) != -1 ){
		printf("Semaforo consumidor abierto\n");
	}else{
		printf("Hubo un problema al abrir el semaforo consumidor\n");
	}	
}

int main(){
	int *tmp;
	int sem_id;
	struct sembuf sb = {0, -1, 0};

	tmp = alloc();
	sem_id = createSystemV();
	semctl(sem_id, PRODUCTOR, SETVAL, 1);
	semctl(sem_id, CONSUMIDOR, SETVAL, 0);

	for( int i = 0; i < 20; i++ ){
		semWait(sem_id, PRODUCTOR);
		tmp[0]++;
		printf("Productor: %i\n", tmp[0]);
		semSignal(sem_id, CONSUMIDOR);
	}

	//Free Shared Memory
	//shmdt( (char *)tmp );
	//shmctl(id_mem, IPC_RMID, NULL);
	
	return 0;
}
