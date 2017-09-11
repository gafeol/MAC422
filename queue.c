#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


Queue queue_create(){
	Queue q = (Queue) malloc(sizeof(queue));
	q->first = q->last = NULL;
	q->size = 0;
	return q; 
}

int queue_empty(Queue q){
	return (q->size == 0);
}

void *head(Queue q){
	if(q->first == NULL) return NULL;
	return q->first->value;
}

void *tail(Queue q){
	if(q->last == NULL) return NULL;
	return q->last->value;
}

void queue_push(Queue q, void *value){
	Node new_node = (Node) malloc(sizeof(Node));
	new_node->value = value;
	new_node->next = NULL;

	q->size++;

	if(q->last == NULL)
		q->first = new_node;
	else
		q->last->next = new_node;

	q->last = new_node;
}

void queue_pop(Queue q){
	Node top = q->first;
	q->first = top->next; 
	if(q->first == NULL)
		q->last = NULL;
	free(top);
	q->size--;
}

void queue_delete(Queue q){
	while(q->size != 0)
		queue_pop(q);
	free(q);
}

int queue_size(Queue q){
	return q->size;
}

 
/*
int main(){
	Queue q = queue_create();
	int *v;
	v = malloc(2*sizeof(int));
	v[0] = 1;
	v[1] = 2;
	queue_push(q, v);
	queue_push(q, v+1);
	assert(queue_size(q) == 2);
	assert(head(q) == v);
	assert(tail(q) == v+1);
	assert(*(int *)head(q) == v[0]);
	queue_pop(q);
	assert(tail(q) == head(q));
	queue_pop(q);
	assert(tail(q) == NULL);
	assert(head(q) == NULL);
	assert(queue_size(q) == 0);

	queue_delete(q);
	free(v);
}


*/
