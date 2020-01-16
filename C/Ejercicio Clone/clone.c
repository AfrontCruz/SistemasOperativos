#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>


int clone_usr(){
    printf("Soy el hijo: %d  mi padre: %d\n",getppid() ,getpid() );
    return 0;
}


int main(int argc,char** argv){
    void *pila_hijo;
	int i = 0;
	int h;
	pila_hijo = malloc(100*sizeof(void));

	printf("Soy el pocreso: %d\n",getpid());

  	if( clone( clone_usr(), pila_hijo, CLONE_FILES, 0) > 0 )
 		printf( "Soy el proceso %d, mi hijo es: %d\n",getpid(), getppid());
	else
		perror("Existe un error");

 	return 0;
}
