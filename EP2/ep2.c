#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "global.h"
#include "ciclista.h"
#include "pista.h"
#include "queue.h"


Queue *resultados;
pthread_mutex_t **mutex_resultados;

void *run_process(void * ii){
	int i = *((int *)ii);

	while(1){
		/* Thread vai simular dt ms da corrida para o ciclista i */
		int pos = ((int)ciclistas[i].dist)%tam_pista;

		/* Vetor de mutexes pra lockar a linha e a proxima pra quando eu for colocar meu cara na pista */
		pthread_mutex_lock(pista[pos].linha);
		pthread_mutex_lock(pista[(pos+1)%tam_pista].linha);
		/* Atualizar dist, matriz de pista com a nova posição */
		ciclista_avanca(i);
		pthread_mutex_unlock(pista[pos].linha);
		pthread_mutex_unlock(pista[(pos+1)%tam_pista].linha);

		/* Verificacao de 20 pontos se o ind[0] ta uma rodada a mais que o segundo maior (ind[1]) */
		printf("PA!\n");
		printf("%d parou no arrive\n", i);
	
		/* Atualizar o lap e tempo */
		if(ciclistas[i].dist/tam_pista > ciclistas[i].voltas){
			ciclistas[i].voltas++;
			ciclistas[i].completou_volta = 1;

			int novalap = ciclistas[i].voltas;
			/* Colocar o cara na lista ligada da lap ciclistas[i].volta */
			pthread_mutex_lock(mutex_resultados[novalap]);
			queue_push(resultados[novalap], i);
			printf("Ciclista %d terminou como o %d-esimo da %d-esima volta\n", i, queue_size(resultados[novalap]), novalap-1);
			pthread_mutex_unlock(mutex_resultados[novalap]);
		}

		if(testa_quebrou(i)){
			pthread_mutex_lock(pista[pos].linha);
			remove_ciclista_pista(i); 
			pthread_mutex_unlock(pista[pos].linha);
		}
		sorteia_velocidade(i); 
		
		/* Thread sorteia velocidade da proxima rodada */
		ciclistas[i].completou_volta = 0;
		pthread_mutex_unlock(ciclistas[i].arrive);
		printf("%d passou no arrive\n", i);
		printf("%d parou no continue\n", i);
		pthread_mutex_lock(ciclistas[i].cont);
		printf("%d passou no continue\n", i);
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

void inicializa_ciclistas(int n){
	ciclistas = malloc(n*(sizeof(ciclista)));

	int i;
	for(i=0;i<n;i++){
		ciclistas[i].id = malloc(sizeof(int));	
		*ciclistas[i].id = i;
		ciclistas[i].dist = -(i/10);	
		ciclistas[i].voltas = 0;
		ciclistas[i].tempo = 0;
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		ciclistas[i].destruido = 0;
		ciclistas[i].terminou = 0;
		ciclistas[i].completou_volta = 0;
		create_thread(i);
		printf("Criou thread\n");

	}
}

int cmp(const void *aa, const void *bb){
	int a = *(int *)aa;
	int b = *(int *)bb;
	return (ciclistas[a].dist > ciclistas[b].dist || (ciclistas[a].dist == ciclistas[b].dist && ciclistas[a].raia < ciclistas[b].raia));
}

int vai_rodar(int i){
	return !(ciclistas[i].destruido == 1 ||
				ciclistas[i].dist >= tam_pista);
}

void barreira_threads(){
	int i;
	for(i = 0;i < num_ciclistas;i++){
		if(vai_rodar(i)) continue;
		pthread_mutex_lock(ciclistas[i].arrive);
	}
}

void libera_threads(){
	int i;
	for(i = 0;i < num_ciclistas;i++){
		if(vai_rodar(i)) continue;
		pthread_mutex_unlock(ciclistas[i].cont);
	}
}

int main(int argc, char* argv[]){
	printf("fodeu\n");
	tam_pista = atoi(argv[1]);
	num_ciclistas = atoi(argv[2]);
	num_voltas = atoi(argv[3]);

	sorteio_ciclista_sortudo();

	inicializa_pista(tam_pista);

	/* Queue e mutex de resultados */
	resultados = malloc(num_voltas*sizeof(Queue));
	mutex_resultados = malloc(num_voltas*sizeof(pthread_mutex_t*));
	for(int i = 0;i < num_voltas;i++){
		resultados[i] = queue_create();
		pthread_mutex_init(mutex_resultados[i], NULL);
	}

	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	int cnt = 2;
	while(cnt){
		printf("Coordenador parou no arrive\n");
		barreira_threads();

		//Coordenador
		//Barreira de Sincronizacao
		// Sorteia velocidades e decide acoes
		printf("Coordenador parou no continue\n");
		int i = 0;
		while(pthread_mutex_unlock(ciclistas[i++].cont));
		printf("Coordenador passoou no continue\n");
		printf("Sincronizou!");
		cnt--;
	}
	return 0;
}

/* TODO
	Criar queues de colocacoes
	Fazer a contagem de pontos por volta
	Verificacao de size de queue = n pra imprimir e dar free
*/
