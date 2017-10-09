#ifndef ALEATORIO_H
#define ALEATORIO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "global.h"

int sorteio(int p)
{
	if(semente == 0) {
		srand((unsigned)time(NULL));
		semente = 1;
	}
	int x = rand() % 100;
	return (x < p) ? 1 : 0;  
}

void sorteio_ciclista_sortudo()
{
	if(sorteio(10))
		ciclista_sortudo = (rand() % num_ciclistas);
	else 
		ciclista_sortudo = -1;
}

#endif
