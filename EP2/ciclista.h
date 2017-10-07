#ifndef CICLISTA_H
#define CICLISTA_H

#include <stdio.h>
#include <pthread.h>

typedef struct ciclista{
	int* id;
	double dist;
	int voltas;
	int tempo;
	int raia; /* de 0 a 9 a raia do ciclista */
	int velocidade; /* em km/h */
	int destruido;
	int terminou;
	pthread_t *thread;
	pthread_mutex_t *arrive;
	pthread_mutex_t *cont;
} ciclista;

ciclista *ciclistas;

int vai_rodar(int);
void inicializa_ciclistas(int);
void destroi_ciclista(int);

#endif
