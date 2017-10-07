#include <stdlib.h>
#include <stdio.h>

#include "ciclista.h"

void destroi_ciclista(int i){
	ciclistas[i].destruido = 1;
}

void inicializa_ciclistas(int n){
	ciclistas = malloc(num_ciclistas*(sizeof(ciclista)));

	int i;
	for(i=0;i<num_ciclistas;i++){
		ciclistas[i].id = malloc(sizeof(int));	
		*ciclistas[i].id = i;
		ciclistas[i].dist = -(i/10);	
		ciclistas[i].voltas = 0;
		ciclistas[i].tempo = 0;
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		ciclistas[i].destruido = 0;
		ciclistas[i].terminou = 0;
		create_thread(i);
		printf("Criou thread\n");

	}
}

int vai_rodar(int i){
	return !(ciclistas[i].destruido == 1 || 
			ciclistas[i].terminou == 1);
}
