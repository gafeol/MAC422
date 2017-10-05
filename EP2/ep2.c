#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ciclista.h"

int tam_pista, num_ciclistas, num_voltas;

int **pista;

void *run_process(void * ii){
	int i = *((int *)ii);
	while(ciclistas[i].dist < 1){
		printf("PA!\n");
		printf("%d parou no arrive\n", i);
		pthread_mutex_unlock(ciclistas[i].arrive);
		printf("%d passou no arrive\n", i);

		printf("%d parou no continue\n", i);
		pthread_mutex_lock(ciclistas[i].cont);
		printf("%d passou no continue\n", i);
		ciclistas[i].dist += 10;
	}
}

void create_thread(int i){
	ciclistas[i].arrive = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(ciclistas[i].arrive, NULL);
	pthread_mutex_lock(ciclistas[i].arrive);
	ciclistas[i].cont = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(ciclistas[i].cont, NULL);
	pthread_mutex_lock(ciclistas[i].cont);
	ciclistas[i].thread = malloc(sizeof(pthread_t));
	pthread_create(ciclistas[i].thread, NULL, run_process, ciclistas[i].id); 
}

int *ind;

int cmp(const void *aa, const void *bb){
	int a = *(int *)aa;
	int b = *(int *)bb;
	return (ciclistas[a].dist > ciclistas[b].dist);
}

int main(int argc, char* argv[]){
	printf("fodeu\n");
	tam_pista = atoi(argv[1]);
	num_ciclistas = atoi(argv[2]);
	num_voltas = atoi(argv[3]);

	pista = malloc(tam_pista*(sizeof (int *)));

	int i = 0;
	for(i = 0;i < tam_pista;i++)
		pista[i] = malloc(10*sizeof(int));

	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));
	ind = malloc(num_ciclistas*(sizeof(ciclista)));

	for(i=0;i<num_ciclistas;i++){
		ind[i] = i;
		ciclistas[i].id = malloc(sizeof(int));	
		*ciclistas[i].id = i;
		ciclistas[i].dist = -(i/10);	
		ciclistas[i].tempo = 0;
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		ciclistas[i].destruido = 0;
		create_thread(i);
		printf("Criou thread\n");

	}
	int cnt = 2;

	while(cnt){
		i = 0;
		printf("Coordenador parou no arrive\n");
		while(pthread_mutex_lock(ciclistas[i++].arrive));
		qsort(ind, num_ciclistas, sizeof(int), cmp);
		for(int a=0;a<num_ciclistas;a++){
				
		}
		//Coordenador
		//Barreira de Sincronizacao
		// Sorteia velocidades e decide acoes
		i = 0;
		printf("Coordenador parou no continue\n");
		while(pthread_mutex_unlock(ciclistas[i++].cont));
		printf("Coordenador passoou no continue\n");
		printf("Sincronizou!");
		cnt--;
	}
	return 0;
}
