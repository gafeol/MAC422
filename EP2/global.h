/****************************************************
Equipe
Nome: Thiago Estrela Montenegro 	NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira     NUSP: 9345370
****************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>

int num_ciclistas, num_voltas, tam_pista, dt;
long long tempo;
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
	long long tempo_chegada;

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
pthread_barrier_t *intencoes, *ciclistas_parados, *imprime;
pthread_mutex_t *quebrado, *aleatorio;

int volta, imprvolta;

#endif
