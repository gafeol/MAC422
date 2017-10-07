#include <stdlib.h>
#include <stdio.h>

#include "ciclista.h"

void destroi_ciclista(int i){
	ciclistas[i].destruido = 1;
}

int vai_rodar(int i){
	return !(ciclistas[i].destruido == 1 || 
				ciclistas[i].terminou == 1);
}
