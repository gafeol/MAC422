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

int head(Queue q){
	if(q->first == NULL) return -1;
	return q->first->value;
}

int tail(Queue q){
	if(q->last == NULL) return -1;
	return q->last->value;
}

void queue_push(Queue q, int value){
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

