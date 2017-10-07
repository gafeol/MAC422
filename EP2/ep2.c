#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ciclista.h"
#include "aleatorio.h"

int tam_pista, num_ciclistas, num_voltas;

int dt;

void *run_process(void * ii){
	int i = *((int *)ii);
	/* Thread vai simular dt ms da corrida para o ciclista i */

	/* Vetor de mutexes pra lockar a linha e a proxima pra quando eu for colocar meu cara na pista */

	while(1){
		/* Atualizar o lap e tempo */
		/* Atualizar dist, matriz de pista com a nova posição */

		printf("PA!\n");
		printf("%d parou no arrive\n", i);

		/* Thread sorteia velocidade da proxima rodada */
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

int *ind;

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

	inicializa_pista(tam_pista);

	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	int cnt = 2;

	while(cnt){
		printf("Coordenador parou no arrive\n");
		barreira_threads();

		/* Verificacao de 20 pontos se o ind[0] ta uma rodada a mais que o segundo maior (ind[1]) */

		/* Verifica se alguem quebrou e atualiza os laps */
		for(int i=0;i<num_ciclistas;i++){
			if(!vai_rodar(i)) continue;	
			if(ciclistas[i].dist/tam_pista > ciclistas[i].voltas){
				ciclistas[i].voltas = ciclistas[i].dist/tam_pista;
				
			}
		}

		/* Arrumar: so faco o sorteio de velocidade se acabou de finalizar uma lap */
		for(int ii=0;ii<num_ciclistas;ii++){
			int i = ind[ii];
			if(!vai_rodar(i)) continue;		
			switch (ciclistas[i].velocidade){
				case 30:
					if(sorteio(70)){
						/* 70% de chance de rodar a 60km/h */
					}
					else{
						/* 30% de chance de rodar a 60km/h */

					}
				break;
				case 60:

				break;
				case 90:

				break;
				default :
					printf("Ciclista %d: Valor inesperado de velocidade %d\n", i, ciclistas[i].velocidade);
			}
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
