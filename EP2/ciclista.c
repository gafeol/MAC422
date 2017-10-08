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
	int j;
	for(j = 0;ciclistas[j].id != NULL;j++){
		ciclistas_ativos += 1 - ciclistas[j].destruido;
	}
	if(ciclistas_ativos <= 5)
		return 0;
	if(sorteio(1)){
		ciclistas[i].destruido = 1;
		return 1;
	}
	return 0;
}

void sorteia_velocidade(int i)
{
	if(!ciclistas[i].completou_volta) return;
	if((num_voltas - ciclistas[i].volta) == 2 && i == ciclista_sortudo) {
		ciclistas[i].velocidade = 90;
		return;
	} 
	if(ciclistas[i].velocidade == 30) {
			if(sorteio(70))
				ciclistas[i].velocidade = 60;
			else
				ciclistas[i].velocidade = 30;
		}
		else {
			if(sorteio(50))
				ciclistas[i].velocidade = 60;
			else
				ciclistas[i].velocidade = 30;
	}
}

void ciclista_avanca(int i)
{
}

int vai_rodar(int i){
	return !(ciclistas[i].destruido == 1 || 
			ciclistas[i].terminou == 1);
}
