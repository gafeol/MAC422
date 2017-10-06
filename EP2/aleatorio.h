#ifndef ALEATORIO_H
#define ALEATORIO_H

#include <stdlib.h>
#include <time.h>

void semente()
{
	srand((unsigned)time(NULL));
}

void sorteio(int p)
{
	int x = rand() % 100;
	return (x < p) ? 1 : 0;  
}

#endif