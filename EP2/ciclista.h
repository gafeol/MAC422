#ifndef CICLISTA_H
#define CICLISTA_H

#include <stdio.h>
#include <pthread.h>
#include "global.h"

int vai_rodar(int);
void inicializa_ciclistas(int);
void destroi_ciclista(int);
void sorteia_velocidade(int);
int testa_quebrou(int);
double distancia_a_percorrer(int,int);
void ciclista_avanca(int);
#endif
