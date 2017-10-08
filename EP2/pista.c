#include <stdio.h>
#include <pthread.h>

#include "pista.h"

void inicializa_pista(int tam){
	pista = malloc(tam*sizeof(matriz_pista));
	int i,j;
	for(i = 0;i < tam;i++) {
		pista[i].raia = malloc(10*sizeof(int));
		for(j = 0; j < 10; i++)
			pista[i].raia[j] = -1;
		pista[i].linha = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(ciclistas[i].linha, NULL);
		pthread_mutex_lock(ciclistas[i].linha);
	}
}

void remove_ciclista_pista(int i)
{
	int raia = ciclistas[i].raia;
	int pos = ((int)ciclistas[i].dist)%tam_pista;
	(pista[pos].raia)[raia] = -1;
}



