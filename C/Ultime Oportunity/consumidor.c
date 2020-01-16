#include "head.h"

char path_1[] = "/dev/loop0";
char path_2[] = "/dev/loop1";
char path_3[] = "/dev/loop2";
char path_4[] = "/dev/loop3";
char path_5[] = "/dev/loop4";

/*
***	Esta función crea la memoría compartida
*/
int id_mem[5];

char *mem1;
char *mem2;
char *mem3;
char *mem4;
char *mem5;

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
	return sem_id;
}

int semAvalible(int sem, int n){
	int num = semctl( sem, n, GETVAL, NULL );
	//printf("Wait Avalible: %i \n", num);
	return num;
}

void semWait(int sem, int n){
	struct sembuf sb = { n, -1, SEM_UNDO };
	if( semop(sem, &sb, 1) != -1 ){
		//printf("Semaforo consumidor bloqueado\n");
	}
	else{
		printf("Hubo un problema al bloquear el semaforo consumidor\n");
	}
}

void semSignal(int sem, int n){
	struct sembuf sb = { n, 1, SEM_UNDO };
	if( semop(sem, &sb, 1) != -1 ){
		//printf("Semaforo productor abierto\n");
		//int a;
	}
	else
		printf("Hubo un problema al abrir el semaforo productor\n");
}

int buscarSem(int id_sem){
	int m = 0;
	while( 1 ){
		if( semAvalible(id_sem, m + 2) > 0 )
			return m + 2;
		m++;
		if( m == 5 )
			m = 0;
	}
	return -1;
}

void escribirEnMem(int id_sem){
	int id = buscarSem(id_sem);
	printf("Consumidor.. Sem %i\n", id );
	semSignal(id_sem, id);
}

int main(){
	mem1 = alloc( path_1, 0 );
	mem2 = alloc( path_2, 1 );
	mem3 = alloc( path_3, 2 );
	mem4 = alloc( path_4, 3 );
	mem5 = alloc( path_5, 4 );

	int proceso, estado;
	char aux;
	int id_sem;
	int m;
	for(int i = 0; i < 10; i++){
		proceso = fork();
		switch( proceso ){
			case -1:
				printf("No se pudo crear el proceso!!!!!!!!!\n");
			case 0:
				aux = 65 + i;
				id_sem = createSystemV(7);

				for( int x = 0; x < 1000; x++ ){
					semWait(id_sem, 1);
					escribirEnMem(id_sem);
					semSignal(id_sem, 0);
				}
				
				printf("C%i.-Estoy terminando bien!\n", i);
				//sleep(5000);
				
				return 0;
			case 1:
				if( i == 9 )
					for( int j = 0; j < 10; j++ ){
						wait( &estado );
					}
				break;
		}
	}
	
	return 0;
}

