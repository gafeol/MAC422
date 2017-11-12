/****************************************************
Equipe
Nome: Thiago Estrela Montenegro 	NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira     NUSP: 9345370
****************************************************/

#ifndef HEAP_H
#define HEAP_H

typedef struct heap_node {
	int priority;
	int element;	
} heap_node;

typedef struct heap {
	heap_node *h[110000];
	int size;
} heap;

typedef heap* Heap;

heap *heap_create();
int heap_empty(heap*);
double heap_min_priority(heap*);
int heap_min_element(heap*);
void heap_push(heap*, double, int);
void heap_pop(heap*);
void heap_destroy(heap*);
void heapify(heap*,int);
void swap(heap_node *a, heap_node *b);

#endif
