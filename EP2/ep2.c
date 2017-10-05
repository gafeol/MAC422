#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ciclista.h"

int tam_pista, num_ciclistas, num_voltas;

int **pista;

ciclista *ciclistas;

void *run_process(void * i){
	
}

int main(int argc, char* argv[]){
	tam_pista = atoi(argv[1]);
	num_ciclistas = atoi(argv[2]);
	num_voltas = atoi(argv[3]);

	pista = malloc(tam_pista*(sizeof (int *)));
	int i = 0;
	for(i = 0;i < tam_pista;i++)
		pista[i] = malloc(10*sizeof(int));

	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	for(i=0;i<num_ciclistas;i++){
		ciclistas[i].id = i;	
		ciclistas[i].arrive = 0;	
		ciclistas[i].cont = 0;	
		ciclistas[i].dist = -(i/10);	
		ciclistas[i].tempo = 0;
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		ciclistas[i].destruido = 0;
		pthread_mutex_init(ciclistas[i].mutex);
		pthread_mutex_lock(ciclistas[i].mutex);
		pthread_create(ciclistas[i].thread, NULL, run_process, (void*)i); 
	}

	while(){
		//Coordenador

		//Barreira de Sincronizacao
		i = 0;
		while(ciclistas[i++].arrive == 0) skip;

		for(i=0;i<num_ciclistas;i++)
			ciclistas[i].arrive = 0;	
		for(i=0;i<num_ciclistas;i++)
			ciclista[i].cont = 1;
	}

	return 0;
}
