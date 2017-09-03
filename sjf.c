#include "process.h"
#include "queue.h"
#include "heap.h"

static timeval start_time;

double sec(timeval t){
	return t.tv_sec + t.tv_usec/1000000.;
}

double running_time(){
	timeval act;
	gettimeofday(&act, NULL);
	return sec(act) - sec(start_time); 
}

void SJF(FILE* input, FILE* output, int ncores){
	char *line = NULL;
	Heap ordered_process = heap_create();
	while(getline(&line, &len, input)){
		Process p = process_line(line);
		heap_push(ordered_process, p->t0, p);
		pthread_mutex_init(process->mutex, NULL);
		pthread_mutex_lock(process->mutex);
		pthread_create(process->thread, NULL, run_process, process);
	}

	Heap running_process = heap_create();

	gettimeofday(&start_time, NULL);
	
	Process current_process = NULL;
	if(!heap_empty(ordered_process)){
		current_process = heap_min_element(ordered_process);
		heap_pop(ordered_process);
	}

	while(current_process != NULL){
		while(!heap_empty(running_process) && running_time() >= heap_min_time(running_process)){
			// processo acabou de rodar
			Process ready = 
		}
		while(current_time() < current_process->t0);

		while(!heap_empty(ordered_process) && running_time() > heap_min_time(ordered_process)){
			Process top = heap_min_element(ordered_process);
			heap_push(next_process, top->dt, top);
			heap_pop(ordered_process);
		}
		while(running_process->size < ncores && !heap_empty(next_process)){
			Process top = heap_min_element(next_process);
			top->cpu running_process->size + 1;	
			pthread_mutex_unlock(top->mutex);
			heap_push(running_process, running_time() + top->dt, top);
			heap_pop(next_process);
		}

		while(running_time() >= heap_min_time(running_process)){
			Process ready = heap_min_element(running_process);
			heap_pop(running_process);
			Process next = 
		}
	}
}
