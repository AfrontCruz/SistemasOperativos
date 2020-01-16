#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	pid_t pid;
	int estado;

	// Creamos un hijo
	if( (pid=fork()) < 0) {
		perror("\nError en el fork");
		exit(-1);

	} else if(pid==0) { //proceso hijo ejecutando el programa
      
		if( (execl("/home/afront/Documentos/SO/C/Ejercicio Clone/./hello","Hola",NULL) < 0 ) ) {
			perror("\nError en el execl");
			exit(-1);
		}

	}
	wait(&estado);
	
	printf("Soy el padre: %d\n", getpid());
	exit(0);
}
