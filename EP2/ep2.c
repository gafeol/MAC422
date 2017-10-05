#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ciclista.h"

int tam_pista, num_ciclistas, num_voltas;

int **pista;

void *run_process(void * ii){
	int i = *((int *)ii);
	while(ciclistas[i].dist < 10){
		pthread_mutex_unlock(ciclistas[i].arrive);
		pthread_mutex_lock(ciclistas[i].cont);
		ciclistas[i].dist += 10;
	}
}

void create_thread(int i){
	pthread_mutex_init(ciclistas[i].arrive, NULL);
	pthread_mutex_lock(ciclistas[i].arrive);
	pthread_mutex_init(ciclistas[i].cont, NULL);
	pthread_mutex_lock(ciclistas[i].cont);
	pthread_create(ciclistas[i].thread, NULL, run_process, ciclistas[i].id); 
}

int main(int argc, char* argv[]){
	printf("Aa\n");
	tam_pista = atoi(argv[1]);
	num_ciclistas = atoi(argv[2]);
	num_voltas = atoi(argv[3]);
	printf("Aa\n");

	printf("Aa\n");
	pista = malloc(tam_pista*(sizeof (int *)));
	printf("Aa\n");

	int i = 0;
	for(i = 0;i < tam_pista;i++)
		pista[i] = malloc(10*sizeof(int));

	printf("Aa\n");
	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	printf("Aa\n");

	printf("Ca\n");
	for(i=0;i<num_ciclistas;i++){
		ciclistas[i].id = malloc(sizeof(int));	
		*ciclistas[i].id = i;
		ciclistas[i].arrive = 0;	
		ciclistas[i].cont = 0;	
		ciclistas[i].dist = -(i/10);	
		ciclistas[i].tempo = 0;
		printf("Aa");
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		printf("Aa");
		ciclistas[i].destruido = 0;
		create_thread(i);
		printf("Ba");
	}

	int cnt = 0;
	while(!cnt){
		//Coordenador
		//Barreira de Sincronizacao
		i = 0;
		printf("Ba");
		while(pthread_mutex_lock(ciclistas[i++].arrive));
		// Sorteia velocidades e decide acoes
		printf("Ba");
		i = 0;
		while(pthread_mutex_unlock(ciclistas[i++].cont));
		printf("Ba");
		printf("Sincronizou!");
		cnt++;
	}
	return 0;
}
