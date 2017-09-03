#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define LCHILD(x) 2*x + 1
#define RCHILD(x) 2*x + 2
#define PARENT(x) (x-1)/2

void swap(heap_node *a, heap_node *b) {
	heap_node tmp = *a;
	*a = *b;
	*b = tmp;
}

heap *heap_create() {
	heap *H = (heap*) malloc(sizeof(heap));
	H->size = 0;
	return H;
}

void heap_destroy(heap *H) {
	for(int i = 0; i < H->size; i++)
		free(H->h[i]);
	free(H);
}

void heap_push(heap *H, double priority, void *element) {
	heap_node *node = (heap_node*) malloc(sizeof(heap_node));
	node->priority = priority;
	node->element = element;
	int i = (H->size)++;
	while(i && node->priority < (H->h[PARENT(i)])->priority) {
		H->h[i] = H->h[PARENT(i)];
		i = PARENT(i);
	}
	H->h[i] = node;
}

void heap_pop(heap *H)
{
	if(H->size) {
		free(H->h[0]);
		H->h[0] = H->h[--(H->size)];
		heapify(H,0);
	}
	else {
		fprintf(stderr,"\nHeap is empty!\n");
	}
}

void heapify(heap* H, int i)
{
	int smallest = i;
	if(LCHILD(i) < H->size && H->h[LCHILD(i)]->priority < H->h[smallest]->priority)
		smallest = LCHILD(i);
	if(RCHILD(i) < H->size && H->h[RCHILD(i)]->priority < H->h[smallest]->priority)
		smallest = RCHILD(i);
	if(smallest != i) {
		swap(H->h[i],H->h[smallest]);
		heapify(H,smallest);
	}
}

void *heap_min_element(heap* H) {
	return H->h[0]->element;
}

double heap_min_time(heap* H) {
	return H->h[0]->priority;
}

int main()
{
	Heap H = heap_create();
	for(int i = 10; i >= 5; i--) {
		heap_push(H,i,NULL);
		printf("%lf\n", heap_min_time(H));
	}
	for(int i = 0; i < 6; i++) {
		printf("%lf\n", heap_min_time(H));
		heap_pop(H);
	}
	heap_destroy(H);
	return 0;
}
