#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void hacerHijos(int niveles, int hijos, int hijo){
	if(niveles == 0)
		return;
	int i,j,estado;
	pid_t proceso;
	for(i = 0; i < hijos; i++){
		proceso = fork();

		switch( proceso ){
			case -1:
				printf("No se pudo crear el hijo\n");
				_exit(-1);
			case 0:
				printf("Soy el hijo %d mi padre es: %d\n", getpid(), getppid());
				if(hijo == 0){
					hacerHijos(niveles-1, hijos + 1, 0);
				}
				else
					hacerHijos(niveles-1, hijos, 1);
				_exit(0);
				break;
			default:
				if(i == (hijos - 1)){
					for(j = 0; j < hijos; j++){
						wait(&estado);
					}
				}
				break;
		}
	}
}

int main(){
	int niveles;

	printf("Introduce los niveles: ");
	scanf("%d",&niveles);
	pid_t proceso;
	int estado,i,j;

		for(i = 0;i < 2;i++){
			proceso = fork();
			switch(proceso){	
				case -1:
					printf("No se pudo crear el hijo\n");
					_exit(-1);
				case 0:
					if(i == 0){
						printf("Soy el hijo izquierdo  %d mi padre es: %d\n", getpid(), getppid());
						hacerHijos(niveles - 1, 2, 0);
					}
					else{
						printf("Soy el hijo derecho %d mi padre es: %d\n", getpid(), getppid());
						hacerHijos(niveles - 1, 3, 1);
					}
					_exit(0);
					break;
				default:
					if(i == 1){
						printf("Soy el padre: %d\n",getpid());
						for(j = 0;j < 2;j++){
							wait(&estado);
						}
					}
			}
		}
	return 0;
}