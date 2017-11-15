/****************************************************
Equipe
Nome: Thiago Estrela Montenegro 	NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira     NUSP: 9345370
****************************************************/

#ifndef LISTA_H
#define LISTA_H


struct node{
	int p;
	int ini;
	int	fim;
	node *next;
};

struct lista{
	struct node *first;
	int size;
};


typedef node* Node;
typedef lista* Lista;

extern Lista L;

Lista lista_create();

int lista_empty(Lista);

int head(Lista);

int lista_push(Lista, int, int);

void lista_pop(Lista);

void lista_erase(Lista, int);

void lista_delete(Lista);

int lista_size(Lista);

#endif
