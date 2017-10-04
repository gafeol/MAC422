#include <stdio.h>

struct ciclista{
	int id;
	int arrive, cont;
	double dist;
	int tempo;
	int raia; /* de 0 a 9 a raia do ciclista */
	int velocidade; /* em km/h */
	int destruido;
	pthread_t *thread;
	pthread_mutex_t *mutex;
}
