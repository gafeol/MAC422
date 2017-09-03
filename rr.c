#include "process.h"
#include "queue.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct exec_node {
	double exe_time;
	process *proc;
} exec_node;

typedef exec_node* Exec_node;
typedef struct timeval timev;
static timev start_time;

double sec(timev t){
	return t.tv_sec + t.tv_usec/1000000.;
}

double running_time(){
	timev act;
	gettimeofday(&act, NULL);
	return sec(act) - sec(start_time); 
}

static void *run_process(Process p){
	pthread_mutex_lock(p->mutex);
	struct timespec tim, tim2;
	tim.tv_sec = (long) p->dt;
	tim.tv_nsec = (long) 1000000*(p->dt - tim.tv_sec);
	nanosleep(&tim, &tim2);
	p->done = 1;
	return NULL;
}

void RR(FILE* input, FILE* output, int ncores){
	char *line = NULL;
	Queue cpu_livre = queue_create();
	int *core;
	core = malloc((ncores+1)*sizeof(int));
	double quantum = 0.1;
	int id;
	Exec_node en = malloc(sizeof(exec_node));
	for(id = 1; id <= ncores; id++) {
		core[id] = id;
		queue_push(cpu_livre, core+id);
	}
	size_t len = 0;
	Heap ordered_process = heap_create();
	while(getline(&line, &len, input) != -1){
		Process process = process_line(line);
		heap_push(ordered_process, process->t0, process);
		pthread_mutex_init(process->mutex, NULL);
		pthread_mutex_lock(process->mutex);
		pthread_create(process->thread, NULL, run_process, process);
	}

	Heap running_process = heap_create();
	Queue awaiting_process = queue_create();


	gettimeofday(&start_time, NULL);
	
	Process current_process = NULL;
	if(!heap_empty(ordered_process)){
		current_process = heap_min_element(ordered_process);
		heap_pop(ordered_process);
	}

	while(current_process != NULL || running_process->size != 0){
		/*while(!heap_empty(running_process) && running_time() >= heap_min_time(running_process)){
			// processo acabou de rodar
			Process ready = 
		}*/

		//while(current_time() < current_process->t0 && !heap_empty(running_process) && heap_min_time(running_process) <= running_time());

		while(current_time() >= current_process->t0) {
			double exe_time = 0;
			if(current_process->dt >= quantum)
					exe_time = quantum;
				else
					exe_time = current_process->dt;
			queue_push(awaiting_process, exe_time, current_process);
			if(heap_empty(ordered_process)) break;
			current_process = heap_min_element(ordered_process);
			heap_pop(ordered_process);
		}

		while(!heap_empty(running_process) && running_time() > heap_min_time(running_process)){
			Process top = heap_min_element(running_process);
			while(!top->done);
			if(top->dt > 0) {
				double exe_time = 0;
				if(top->dt >= quantum)
					exe_time = quantum;
				else
					exe_time = top->dt;
				top->dt -= exe_time;
				en->exe_time = exe_time;
				en->proc = top;
				top->done = 0;
				pthread_mutex_lock(top->mutex);
				pthread_create(top->thread,NULL,run_process,top);
				queue_push(awaiting_process, en);
				heap_pop(running_process);
			}
		}

		while(!(awaiting_process->size == 0) && (heap->size < ncores)){
			id = *head(cpu_livre);
			queue_pop(cpu_livre);
			process *p = head(awaiting_process)->proc;
			double exe_time = head(awaiting_process)->exe_time;
			p->cpu = id; 
			pthread_mutex_unlock(p->mutex);
			heap_push(running_process,running_time() + exe_time, p);
			queue_pop(awaiting_process);
		}
	}
	free(cores);
	free(en);
	return 0;
}
