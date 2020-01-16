
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>

int memid;

sem_t *sem_creator(char *path,int value)
{
	sem_t *sem;
	sem=sem_open(path,O_CREAT| O_EXCL,0666,value);
	if(sem==SEM_FAILED)
	{
		sem=sem_open(path,0666);
		fprintf(stderr,"Hola ligado del semaforo %d",sem);
		//printf("%s",errno);
	}
	else
		printf("Hola cree el semaforo: %d",sem);
	return sem;
	
}

int *alloc()
{
	int *tmp;
	key_t shamem;
	shamem=ftok("/dev/null", 33);
	if( shamem == -1 ){
		fprintf(stderr,"Hubo un error al iniciar la llave");
		exit(1);
	}
	memid=shmget(shamem,sizeof(int),IPC_CREAT|IPC_EXCL|0777);
	if( memid == -1 ){
		memid=shmget(shamem,sizeof(int),0777);
		if(memid==-1)
			fprintf(stderr,"Hubo un error al obtener el id");
	}
	tmp=(int *)shmat(memid,NULL,0);
	if( tmp == NULL){
		perror("No se consiguio la memoria compartida..");
			
	}
	return tmp;
}

int main()
{
	sem_t *sem0=sem_creator("/dev/shm/sem.sem0",1); //probar con tmp
	sem_t *sem1=sem_creator("/dev/shm/sem.sem1",0); //probar con tmp
	//memoria
	int *tmp=alloc();
	
	for(int i=0;i<20;i++)
	{
		sem_wait(sem0);
		*tmp=i;
		sem_post(sem1);
	}
	
	//free
	//shmdt((char *)tmp);
	//shmctl(memid,IPC_RMID,NULL);
	
	
}
