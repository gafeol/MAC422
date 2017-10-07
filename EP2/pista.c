#include <stdio.h>
#include <pthread.h>

void inicializa_pista(int tam){
	pista = malloc(tam*sizeof(matriz_pista));
	int i;
	for(i = 0;i < tam;i++){
		pista[i].raia = malloc(10*sizeof(int));
		pista[i].linha = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(ciclistas[i].linha, NULL);
		pthread_mutex_lock(ciclistas[i].linha);
	}
}
