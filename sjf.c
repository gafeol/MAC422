#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#include "process.h"
#include "queue.h"
#include "heap.h"

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

void finish_process(Heap running_process, Queue cpu_livre){
	// caso algum processo que esta rodando ja tenha terminado, o finish process tira ele da heap e libera a cpu
	while(!heap_empty(running_process) && heap_min_element(running_process)->done == 1){
		// processo acabou de rodar
		Process ready = heap_min_element(running_process);
		heap_pop(running_process);
		//libera cpu
		queue_push(cpu_livre, &ready->cpu);
	}
}

static void *run_process(void *p){
	pthread_mutex_lock(proc->mutex);
	struct timespec tim, tim2;
	tim.tv_sec = (long) p->dt;
	tim.tv_nsec = (long) 1000000*(p->dt - tim.tv_sec);
	nanosleep(&tim, &tim2);
	p->done = 1;
	return NULL;
}

void SJF(FILE* input, FILE* output, int ncores){

	Queue cpu_livre = queue_create(); 
	int *cores;
	cores = malloc((ncores+1)*sizeof(int));
	int id;
	for(id = 1;id <= ncores;id++){
		cores[id] = id;
		queue_push(cpu_livre, cores+id);
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	Heap ordered_process = heap_create();
	while((read = getline(&line, &len, input)) != -1){
		Process p = process_line(line);
		heap_push(ordered_process, p->t0, p);
		pthread_mutex_init(p->mutex, NULL);
		pthread_mutex_lock(p->mutex);
		pthread_create(p->thread, NULL, run_process, p);
	}

	Heap running_process = heap_create();

	Heap next_process = heap_create();

	gettimeofday(&start_time, NULL);

	Process current_process = NULL;
	if(!heap_empty(ordered_process)){
		current_process = heap_min_element(ordered_process);
		heap_pop(ordered_process);
	}

	while(!heap_empty(next_process) || !heap_empty(ordered_process)){

		if(heap_empty(next_process))
			current_process = heap_min_element(ordered_process);
		else
			current_process = heap_min_element(next_process);

		finish_process(running_process, cpu_livre);

		while(current_time() < current_process->t0)
			finish_process(running_process, cpu_livre);
		//se nao posso ainda adicionar o proximo processo ou nao tenho cpu livre, espero

		//boto todos que ja podem ser executador na next_process
		while(!heap_empty(ordered_process) && running_time() > heap_min_time(ordered_process)){
			Process top = heap_min_element(ordered_process);
			heap_push(next_process, top->dt, top);
			heap_pop(ordered_process);
		}

		while(!heap_empty(next_process) && !queue_empty(cpu_livre)){
			Process top = heap_min_element(next_process);

			top->cpu = *(int *)head(cpu_livre);	
			queue_pop(cpu_livre);

			pthread_mutex_unlock(top->mutex);
			heap_push(running_process, running_time() + top->dt, top);
			heap_pop(next_process);
		}
	}
}
