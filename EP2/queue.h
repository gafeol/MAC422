#ifndef QUEUE_H
#define QUEUE_H

typedef struct node{
	int value;
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

void queue_push(Queue, int);

void queue_pop(Queue);

void queue_delete(Queue);

int queue_size(Queue);

#endif
