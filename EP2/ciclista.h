#ifndef CICLISTA_H
#define CICLISTA_H

#include <stdio.h>

typedef struct ciclista{
	int id;
	int arrive, cont;
	double dist;
	int tempo;
	int raia; /* de 0 a 9 a raia do ciclista */
	int velocidade; /* em km/h */
	int destruido;
	pthread_t *thread;
	pthread_mutex_t *mutex;
};

void destroi_ciclista(int);

#endif
