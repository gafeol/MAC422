#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>

int num_ciclistas, num_voltas, tam_pista, dt, tempo;
int ciclista_sortudo;
int volta_atual;
int quebrou = 0;
int ciclistas_ativos;

int debug;

int semente = 0;

typedef struct ciclista{
	int* id;
	double dist;
	int voltas;
	int completou_volta;
	int tempo;
	int raia; /* de 0 a 9 a raia do ciclista */
	int velocidade; /* em km/h */
	int destruido;
	int terminou;
	int pontuacao;
	int max_volta_extra;
	int tempo_chegada;

	int avanca;

	pthread_t *thread;
	pthread_mutex_t *arrive;
	pthread_mutex_t *cont;
} ciclista;

ciclista *ciclistas;

typedef struct matriz_pista{
	int tam_pista;
	int *raia;
	pthread_mutex_t *linha;
} matriz_pista;

matriz_pista *pista, *pista_aux;

pthread_barrier_t *arrive, *barreira_andou, *cont; 
ptread_barrier_t *intencoes, *ciclistas_parados, *ciclistas_avancam;
pthread_mutex_t *quebrado;

int volta, imprvolta;
pthread_mutex_t *mutex_volta;

#endif
