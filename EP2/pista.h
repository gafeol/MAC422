#ifndef PISTA_H
#define PISTA_H

#include <stdio.h>
#include <pthread.h>
#include "global.h"

void inicializa_pista(int);
void remove_ciclista_pista(int);
void desloca_ciclista_pista(int,int);

#endif

