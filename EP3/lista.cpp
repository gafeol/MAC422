/****************************************************
Equipe
Nome: Thiago Estrela Montenegro 	NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira     NUSP: 9345370
****************************************************/

#include <bits/stdc++.h>

#include "lista.h"
#include "global.h"
#include "processo.h"

#define debug(args...) //fprintf(stderr, args);


Lista L;

Lista lista_create(){
	Lista q = (Lista) malloc(sizeof(lista));
	q->first = NULL;
	q->size = 0;
	return q;
}

int lista_empty(Lista q){
	return (q->size == 0);
}

int head(Lista q){
	if(q->first == NULL) return -1;
	return q->first->p;
}

int inter(Node u, Node v){
	// Espaco livre (em paginas) entre os dois nos u e v
	int l = 0, r = ceil(virt, tam_pag)-1;
	if(u != NULL) l = u->fim + 1;
	if(v != NULL) r = v->ini - 1;

	return r - l + 1;
}

void put(Lista q, Node u, Node v){
	//debug("put lista\n");
	if(u == NULL){
		v->next = q->first;
		q->first = v;
		v->ini = 0;
	}
	else{
		v->next = u->next;
		u->next = v;
		v->ini = u->fim+1;
	}

	v->fim = v->ini + ceil(processos[v->p].b, tam_pag) - 1;
}

int lista_push(Lista q, int p, int alg_aloc){
	debug("lista_push %d %d\n", p, alg_aloc);
	// Coloca o processo p na posicao certa
	Node new_node = (Node) malloc(sizeof(node));
	new_node->p = p;

	debug("lista size antes %d\n", q->size);
	q->size++;
	debug("lista size depois %d\n", q->size);

	Node u = q->first;
	Node lst = NULL;

	int sz = ceil(processos[p].b, tam_pag);

	int best = INT_MAX;
	Node ibest = NULL;
	int worst = INT_MIN;
	Node iworst = NULL;

	while(1){
		int esp = inter(lst, u);
		if(esp >= sz){
			if(best > esp){
				best = esp;
				ibest = lst;
			}
			if(worst < esp){
				worst = esp;
				iworst = lst;
			}
		}
		if(u == NULL) break;
		lst = u;
		u = u->next;
	}

	if(alg_aloc == 1) //Best fit
		put(q, ibest, new_node);
	else 			  //Worst fit
		put(q, iworst, new_node);

	return new_node->ini;
}

void lista_insert(Lista q, int p, int pos_virt){
	// Coloca o processo p na posicao certa
	Node new_node = (Node) malloc(sizeof(node));
	new_node->p = p;

	q->size++;

	Node u = q->first;
	Node lst = NULL;

	int sz = ceil(processos[p].b, tam_pag);


	while(u != NULL){
		if(u->ini >= pos_virt)
			break;
		lst = u;
		u = u->next;
	}

	put(q, lst, new_node);

	assert(new_node->ini == pos_virt);
	assert(new_node->fim == pos_virt + sz - 1);

	new_node->ini = pos_virt;
	new_node->fim = pos_virt + sz - 1;
}

void lista_pop(Lista q){
	Node top = q->first;
	q->first = top->next;
	free(top);
	q->size--;
}

void lista_erase(Lista q, int p){
	// Apaga o processo p da lista ligada
	Node u = q->first;
	Node ult = NULL;
	while(u != NULL && u->p != p){
		ult = u;
		u = u->next;
	}
	assert(u != NULL && "Processo nao encontrado em lista ligada");
	if(ult == NULL)
		q->first = u->next;
	else
		ult->next = u->next;

	free(u);
	q->size--;
}

void lista_delete(Lista q){
	while(q->size != 0)
		lista_pop(q);
	free(q);
}

int lista_size(Lista q){
	return q->size;
}
