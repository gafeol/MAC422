#ifndef PROCESS_H
#define PROCESS_H


#include <pthread.h>


typedef struct process{
	float t0, dt, deadline;

	float elapsed; /* tempo que este processo rodou ate agora */

	char *name;

	int priority;

	pthread_t *thread;
	pthread_mutex_t *mutex;
	
} process;

typedef process* Process;

process *process_create(float, float, float, char*);

process *process_line(char*);

void process_delete(process*);

#endif
