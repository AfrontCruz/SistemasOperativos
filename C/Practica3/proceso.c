#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "proceso.h"
#include <time.h>
#include <stdio.h>

int main (){
  int filas1, columnas1, filas2, columnas2;
  printf ("Ingresa el numero de filas de la primer matriz:");
  scanf ("%i", &filas1);
  printf ("Ingresa el numero de columnas de la primer matriz:");
  scanf ("%i", &columnas1);
  printf ("Ingresa el numero de filas de la segunda matriz:");
  scanf ("%i", &filas2);
  printf ("Ingresa el numero de columnas de la segunda matriz:");
  scanf ("%i", &columnas2);

  if( columnas1 != filas2 ){
    printf("No se puede hacer la multiplicacion\n");
    exit(1);
  }

  int **m1, **m2, **pipes;
  int p[2];
  m1 = crearMatriz (filas1, columnas1);
  m2 = crearMatriz (filas2, columnas2);
    
  llenarMatriz (m1, filas1, columnas1);
  llenarMatriz (m2, filas2, columnas2);

  printf ("Las matrices generadas son:\nMatriz 1:\n");
  imprimirMatriz (m1, filas1, columnas1);
  printf ("\nMatriz 2:\n");
  imprimirMatriz (m2, filas2, columnas2);
  printf ("\n");
  int no_p, no_fila_hijos, no_fila_uh;

  dividirProcesos(&no_p, &no_fila_hijos, &no_fila_uh, filas1);
  int rango = no_fila_hijos;

  /*pipes = crearMatriz(no_p, 2);
  
  for( int z = 0; z < no_p; z++){
    if( pipe( pipes[z] ) == -1 ){
      perror("Hubo un error al crear el pipe");
    }
  }*/
  pipe(p);

  pid_t proceso;
  int estado, i, j;
  for (i = 0; i < no_p; i++){
    proceso = fork ();
    int **sub;
    sub = crearMatriz(no_fila_uh, columnas2);
    switch (proceso){
    case -1:
      perror("No se pudo crear el proceso");
      break;
    case 0:
      //close(pipes[i][0]);
      close(p[0]);
      if( i == (no_p - 1) ){
        //sub = crearMatriz(no_fila_uh, columnas2);
        sub = multiplicarMatrices(m1, m2, rango, i, no_fila_uh, filas2, columnas2);
        //write(pipes[i][1],sub,sizeof(int[no_fila_uh][columnas2]));
        write(p[1],sub,sizeof(int[no_fila_uh][columnas2]));
      }
      else{
        //sub = crearMatriz(no_fila_hijos, columnas2);
        sub = multiplicarMatrices(m1, m2, rango, i, no_fila_hijos, filas2, columnas2);
        //write(pipes[i][1],sub,sizeof(int[no_fila_hijos][columnas2]));
        write(p[1],sub,sizeof(int[no_fila_hijos][columnas2]));
      }
      _exit (0);
      break;
    default:
        //close(pipes[i][1]);
        close(p[1]);
        printf("La matriz recibida es:\n");
        if(i == (no_p - 1)){
          //read(pipes[i][0],sub,sizeof(int[no_fila_uh][columnas2]));  
          read(p[0],sub,sizeof(int[no_fila_uh][columnas2]));  
          imprimirMatriz(sub,no_fila_uh,columnas2);
        }
        else{
          //read(pipes[i][0],sub,sizeof(int[no_fila_hijos][columnas2]));
          read(p[0],sub,sizeof(int[no_fila_hijos][columnas2]));
          imprimirMatriz(sub,no_fila_hijos,columnas2);
        }
        printf("\n");

        if (i == (no_p - 1)){
          for (j = 0; j < no_p; j++)
            wait (&estado);
        }
    }
  }

/*
        if (i == (no_p - 1)){
          for (j = 0; j < no_p; j++)
            wait (&estado);

          for(j = 0;j < no_p;j++){
            close(pipes[j][1]);
            printf("La matriz recibida es:\n");
            if(j == (no_p - 1)){
              read(pipes[j][0],sub,sizeof(int[no_fila_uh][columnas2]));  
              imprimirMatriz(sub,no_fila_uh,columnas2);
            }
            else{
              read(pipes[j][0],sub,sizeof(int[no_fila_hijos][columnas2]));
              imprimirMatriz(sub,no_fila_hijos,columnas2);
            }
            printf("\n");
          }
        }
        break;
      }
      */
  //else
    //printf ("No se puede hacer la multiplicacion\n");

  return 0;
}

int **crearMatriz(int nfilas, int ncolumnas){
  int **matriz;
  matriz = (int **) malloc (nfilas * sizeof (int *));
  int i;
  if( matriz != NULL ){
    for (i = 0; i < nfilas; i++){
      *(matriz + i) = (int *) malloc (ncolumnas * sizeof (int));
      if( *(matriz + i) == NULL )
        break;    /* Si no se pudo reservar una columna rompe el ciclo */
    }

    if (i != nfilas){
      printf ("Alguna columna no pudo ser creada...\n");
      /* Liberamos las columnas y filas */
      while (i > 0)
        free (*(matriz + i));
      free (matriz);
      matriz = NULL;
    }
  }
  else
    printf ("No se pudo reservar memoria...\n");

  return matriz;
}

void llenarMatriz (int **matriz, int nfilas, int ncolumnas){
  time_t t;
  srand ((unsigned) time (&t));
  int i, j;
  for ( i = 0; i < nfilas; i++ )
      for( j = 0; j < ncolumnas; j++ )
        matriz[i][j] = rand () % 4;
}

void imprimirMatriz (int **m, int nf, int nc){
  int i, j;
  for ( i = 0; i < nf; i++ ){
    printf ("\t ");
    for (j = 0; j < nc; j++)
      printf ("%c ", m[i][j]);
    printf ("\n");
  }
}

void dividirProcesos(int *np, int *nfh, int *nfuh, int nf){
  do{
      printf ("Introduce el numero de procesos:\n");
      scanf ("%d", np);
  }while ( (*np < 0) || (*np > nf) );

  *nfh = (int) (nf / (*np));
  if( nf % (*np) == 0)
    *nfuh = *nfh;
  else
    *nfuh = nf - (*nfh)*(*np - 1);
}

void insertarValor(int **matriz, int valor, int i, int j){
  matriz[i][j] = valor;
}


int **multiplicarMatrices (int **m1, int **m2, int rango, int id, int nf, int filas2, int columnas2){
  int **mr;
  mr = crearMatriz (nf, columnas2);
  int i, j, k, celda;

  for (i = 0; i < nf; i++){
    for (k = 0; k < columnas2; k++){
      celda = 0;
      for (j = 0; j < filas2; j++)
        celda += m1[ (((rango - 1) * id) + id) + i][j] * m2[j][k];

      insertarValor(mr, celda, i, k);
    }
  }
  //printf ("La matriz resultante del proceso %d es:\n", id + 1);
  //imprimirMatriz(mr, nf, columnas2);
  printf ("\n");
  return mr;
}