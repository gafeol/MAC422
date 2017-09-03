#define ifndef QUEUE_H
#define QUEUE_H

typedef struct Node{
	void *value;
	struct Node *next;
} Node;

typedef struct Queue{
	struct Node *first, *last;
	int size;
} Queue;


Queue *queue_create();

void *head(Queue *q);

void *tail(Queue *q);

void queue_push(Queue *q, void *value);

void queue_pop(Queue *q);

void queue_delete(Queue *q);

int queue_size(Queue *q);

#endif
