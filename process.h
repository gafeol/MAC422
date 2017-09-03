#ifndef PROCESS_H
#define PROCESS_H


#include <pthread.h>


typedef struct Process{
	float t0, dt, deadline;

	float elapsed; /* tempo que este processo rodou ate agora */

	char *name;

	int priority;

	pthread_t *thread;
	pthread_mutex_t *mutex;
	
} Process;

Process *process_create(float t0, float dt, float deadline, char *name);

Process *process_line(char *line);

void process_delete(Process *p);

#endif
