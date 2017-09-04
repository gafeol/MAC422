#ifndef HEAP_H
#define HEAP_H

typedef struct heap_node {
	double priority;
	void *element;	
} heap_node;

typedef struct heap {
	heap_node *h[1024];
	int size;
} heap;

typedef heap* Heap;

heap *heap_create();
int heap_empty(heap*);
double heap_min_time(heap*);
void *heap_min_element(heap*);
void heap_push(heap*, double, void*);
void heap_pop(heap*);
void heap_destroy(heap*);
void heapify(heap*,int);
void swap(heap_node *a, heap_node *b);

#endif
