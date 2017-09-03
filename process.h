#ifndef PROCESS_H
#define PROCESS_H


#include <pthread.h>


typedef struct process{
	double t0, dt, deadline;

	double start;

	char *name;

	int priority;

	pthread_t *thread;
	pthread_mutex_t *mutex;

	int cpu;

	int done;
} process;

typedef process* Process;

process *process_create(float, float, float, char*);

process *process_line(char*);

void process_delete(process*);

#endif
