#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char msj[50], msjh[50];
	char buff[80], buffh[80];

	char fin[] = "fin";

	int i = 1, j = 1;

	int f[2], estado;
	int fh[2];
	pid_t p;

	if( pipe(f) == -1 ){
		perror("Error al crear pipe");
		exit(1);
	}
	
	if( pipe(fh) == -1){
		perror("Error al crear pipe");
		exit(1);
	}

	switch( ( p = fork() ) ){
		case -1:
			perror("Error al crear el proceso");
			exit(1);
			break;
		//proceso hijo
		case 0:
			close( f[1] );
			close( fh[0] );
			//printf("Soy el hijo: %i\n", getpid() );
			while(j){
				//Lee el hijo
				int leidos = read( f[0] ,buffh, sizeof(buffh));
				buffh[leidos] = '\0';
				printf("El padre dice: %s",buffh);

				if( strncmp( buffh, fin, 3 ) == 0)
					break;
				
				//Escribe el hijo
				printf("Escribe hijo: ");
				fgets( msjh, 50, stdin );
				write( fh[1],msjh, strlen(msjh) );

				if( strncmp( msjh, fin, 3 ) == 0)
					break;

			}
			break;

		//proceso padre
		default:
			close( f[0] );
			close( fh[1] );
			//printf("Soy el padre: %i\n", getpid() );
			while(i){

				//Escribe el padre
				printf("Escribe padre: ");
				fgets( msj, 50, stdin );
				write( f[1], msj, strlen(msj));

				if( strncmp( msj, fin, 3 ) == 0 )
					break;

				//Lee el hijo
				int leidos = read( fh[0], buff, sizeof(buff) );
				buff[leidos] = '\0';

				printf("El hijo dice: %s",buff);

				if( strncmp( buff, fin, 3 ) == 0 )
					break;
			}
			wait(0);
	}

	return 0;
}
