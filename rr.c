#include "process.h"
#include "queue.h"
#include "heap.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

const double EPS = 1e-4;

typedef struct timeval timev;
static timev start_time;
static FILE *out;

double sec(timev t){
	return t.tv_sec + t.tv_usec/1000000.;
}

double running_time(){
	timev act;
	gettimeofday(&act, NULL);
	return sec(act) - sec(start_time); 
}

static void *run_process(void *pro){
	Process p = pro;
	pthread_mutex_lock(p->mutex);
	
	long runtime = p->dt*1000000;
	if(runtime > 100000. + EPS)
		runtime = 100000;

	p->dt -= runtime/1000000.;

//	printf("antes runtime %ld\n", runtime);
	if(p->start <= -1.)
		p->start = running_time();
	usleep(runtime);
//	puts("depois");

	p->done = 1;
//	puts("p done = 1");
	if(p->dt <= EPS){
//		puts("terminou");
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
		//	printf("while 2 process %s\n", current_process->name);
			current_process->done = 0;
		//	puts("p done = 0");
			queue_push(awaiting_process, current_process);
		//	puts("deu break?");
			if(heap_empty(ordered_process)){
				current_process = NULL;
				break;
			}
		//	puts("nao");
			current_process = heap_min_element(ordered_process);
		//	printf("curr process %s\n", current_process->name);
			heap_pop(ordered_process);
		}

		while(!heap_empty(running_process) && running_time() > heap_min_time(running_process)){
			Process top = heap_min_element(running_process);
			heap_pop(running_process);
		//	printf("top process %s %.3f\n", top->name, top->dt);
			while(!top->done);
			print_cpu_liberation(top, top->cpu);
			queue_push(cpu_livre, &top->cpu);
		//	printf("top process %s done  dt %.3f\n", top->name, top->dt);
			if(top->dt > EPS) {
				// Ainda nao acabou
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
			assert(!queue_empty(cpu_livre));
			id = *((int*) head(cpu_livre));
			queue_pop(cpu_livre);
		//	printf("awaiting process size %d\n", awaiting_process->size);
			Process p = ((Process) head(awaiting_process));

			print_cpu_usage(p, id);

			double exe_time = p->dt;
			if(exe_time > quantum)
				exe_time = quantum;
			//printf("unlock process %s\n", p->name);
			p->cpu = id; 
			pthread_mutex_unlock(p->mutex);
			heap_push(running_process, running_time() + exe_time, p);
			queue_pop(awaiting_process);
		}
	}

	print_context_change();

	free(core);
	free(running_process);
	free(awaiting_process);
	free(ordered_process);
	free(cpu_livre);
}

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


/*

test
1 1 1 pro
2 2 2 pro

dando 28 context changes
pro 2 1
pro2 4 2

*/
