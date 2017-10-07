#ifndef ALEATORIO_H
#define ALEATORIO_H

#include <stdlib.h>
#include <time.h>

int semente = 0;

int sorteio(int p)
{
	if(semente == 0) {
		srand((unsigned)time(NULL));
		semente = 1;
	}
	int x = rand() % 100;
	return (x < p) ? 1 : 0;  
}

#endif
