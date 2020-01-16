#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	printf("%s mi pid es: %i, mi padre es: %i\n", argv[0], getpid(), getppid() );
	return 0;
}
