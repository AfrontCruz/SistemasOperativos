#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, const char *argv[]){
	pid_t pid;
	int estado;
	char *arg[3]={"ls","-l"};
	int proceso = vfork();

	if( proceso == -1 ){
		printf("No se pudo crear el hijo :c\n");
	}
	else if( proceso == 0 ){
		printf("Soy el proceso: %d Mi padre es: %d \n",getpid(),getppid());
		execvp(arg[0],arg);
		perror("Ha ocurrido un error con el exec");
	}
	else{
		printf("Soy el proceso: %d\n",getpid());
		wait(&estado);
	}

	return 0;
}
