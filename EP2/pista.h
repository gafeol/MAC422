#ifndef PISTA_H
#define PISTA_H

#include <stdio.h>
#include <pthread.h>

typedef struct matriz_pista{
	int tam_pista;
	int *raia;
	pthread_mutex_t *linha;
} matriz_pista;

matriz_pista *pista;

void inicializa_pista(int);

#endif
