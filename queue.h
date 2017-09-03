#ifndef QUEUE_H
#define QUEUE_H

typedef struct node{
	void *value;
	struct node *next;
} node;

typedef struct queue{
	struct node *first, *last;
	int size;
} queue;


typedef node* Node;
typedef queue* Queue;

Queue queue_create();

int queue_empty(Queue);

void *head(Queue);

void *tail(Queue);

void queue_push(Queue, void*);

void queue_pop(Queue);

void queue_delete(Queue);

int queue_size(Queue);

#endif
