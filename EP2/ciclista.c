#include <stdlib.h>
#include <stdio.h>

#include "ciclista.h"

void destroi_ciclista(int i){
	ciclistas[i].destruido = 1;
}

void inicializa_ciclistas(int n){
	ciclistas = malloc(n*(sizeof(ciclista)));

	int i;
	for(i=0;i<n;i++){
		ciclistas[i].id = malloc(sizeof(int));	
		*ciclistas[i].id = i;
		ciclistas[i].dist = -(i/10);	
		ciclistas[i].voltas = 0;
		ciclistas[i].tempo = 0;
		ciclistas[i].raia = (i%10);
		ciclistas[i].velocidade = 30;
		ciclistas[i].destruido = 0;
		ciclistas[i].terminou = 0;
		ciclistas[i].completou_volta = 0;
		create_thread(i);
		printf("Criou thread\n");

	}
}

int testa_quebrou(int i){
	if(!ciclistas[i].completou_volta || ciclistas[i].voltas%15 != 0) return 0;
	int ciclistas_ativos = 0;
	int i;
	for(i = 0;ciclistas[i].id != NULL;i++){
		ciclistas_ativos += 1 - ciclistas[i].destruido;
	}
	if(ciclistas_ativos <= 5)
		return 0;
	if(sorteio(1)){
		ciclistas[i].destruido = 1;
		return 1;
	}
	return 0;
}

int vai_rodar(int i){
	return !(ciclistas[i].destruido == 1 || 
			ciclistas[i].terminou == 1);
}
