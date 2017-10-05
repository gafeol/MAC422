#ifndef CICLISTA_H
#define CICLISTA_H

#include <stdio.h>
#include <pthread.h>

typedef struct ciclista{
	int* id;
	double dist;
	int tempo;
	int raia; /* de 0 a 9 a raia do ciclista */
	int velocidade; /* em km/h */
	int destruido;
	pthread_t *thread;
	pthread_mutex_t *arrive;
	pthread_mutex_t *cont;
} ciclista;

ciclista *ciclistas;

void destroi_ciclista(int);

#endif
