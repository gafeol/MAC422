#include "rr.h"

#include "calctime.h"

#include "process.h"
#include "queue.h"
#include "heap.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

static void *run_process(void *pro){
	Process p = pro;
	pthread_mutex_lock(p->mutex);

	long runtime = p->dt*1000000;
	if(runtime > 100000. + 1e-4)
		runtime = 100000;

	p->dt -= runtime/1000000.;

	usleep(runtime);

	if(p->dt <= 1e-4){
		if(running_time() > p->deadline + 1e-4)
			lost_deadline++;
		fprintf(out, "%s %.1f %.1f\n", p->name, running_time(), running_time() - p->t0);
		print_output(p);
	}
	return NULL;
}

void RR(FILE* input, FILE* output, int ncores){
	context_change = 0;
	char *line = NULL;
	out = output;

	Queue cpu_livre = queue_create();
	int *core;
	core = malloc((ncores+1)*sizeof(int));
	double quantum = 0.1;
	int id;
	for(id = 1; id <= ncores; id++) {
		core[id] = id;
		queue_push(cpu_livre, core+id);
	}
	size_t len = 0;
	Heap ordered_process = heap_create();

	while(getline(&line, &len, input) != -1){
		Process process = process_line(line);

		print_trace(process);

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

	while(current_process != NULL || !heap_empty(running_process)){
		while(current_process != NULL && running_time() >= current_process->t0) {
			queue_push(awaiting_process, current_process);
			if(heap_empty(ordered_process)){
				current_process = NULL;
				break;
			}
			current_process = heap_min_element(ordered_process);
			heap_pop(ordered_process);
		}

		while(!heap_empty(running_process) && running_time() > heap_min_time(running_process)){
			Process top = heap_min_element(running_process);
			heap_pop(running_process);
			if(pthread_join(*(top->thread),NULL)) {
				printf("error joining thread\n");
				exit(1);
			}
			queue_push(cpu_livre, &core[top->cpu]);
			print_cpu_liberation(top, top->cpu);
			if(top->dt > 1e-4) {
				// Processo que rodou ainda nao acabou, precisa de mais quantum
				context_change++;
				top->done = 0;
				pthread_mutex_destroy(top->mutex);
				pthread_mutex_init(top->mutex, NULL);
				pthread_mutex_lock(top->mutex);
				pthread_create(top->thread,NULL,run_process,top);
				queue_push(awaiting_process, top);
			}
		}

		while(!queue_empty(awaiting_process) && (running_process->size < ncores)){
			id = *((int*) head(cpu_livre));
			queue_pop(cpu_livre);
			Process p = ((Process) head(awaiting_process));

			print_cpu_usage(p, id);

			double exe_time = p->dt;
			if(exe_time > quantum)
				exe_time = quantum;
			p->cpu = id;
			queue_pop(awaiting_process);
			heap_push(running_process, running_time() + exe_time, p);
			pthread_mutex_unlock(p->mutex);
		}
	}

	print_context_change();

	free(core);
	free(running_process);
	free(awaiting_process);
	free(ordered_process);
	free(cpu_livre);
}

/*
MAIN para testes

int main(){
	print_error = 1;

	FILE *trace = fopen("test.txt", "r"), *output = fopen("saida.txt", "w");
	if(trace == NULL)
		puts("File input open error");
	if(output == NULL)
		puts("File output open error");
	RR(trace, output, 1);
	fclose(trace);
	fclose(output);
}



test
1 1 1 pro
2 2 2 pro

dando 28 context changes
pro 2 1
pro2 4 2

*/
