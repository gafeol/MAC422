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
	struct timespec tim, tim2;
	
	double st = running_time();
	tim.tv_sec = (long) p->dt;
	tim.tv_nsec = (long) 1000000*(p->dt - tim.tv_sec);
	nanosleep(&tim, &tim2);
	p->done = 1;
	fprintf(out, "%s %.1f %.1f\n", p->name, running_time(), running_time() - st); 
	return NULL;
}

void RR(FILE* input, FILE* output, int ncores){
	char *line = NULL;
	out = output;
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
		//printf("process %s\n", current_process->name);
		/*while(!heap_empty(running_process) && running_time() >= heap_min_time(running_process)){
			// processo acabou de rodar
			Process ready = 
		}*/

		//while(current_time() < current_process->t0 && !heap_empty(running_process) && heap_min_time(running_process) <= running_time());

		while(current_process != NULL && running_time() >= current_process->t0) {
			printf("while 2 process %s\n", current_process->name);
			double exe_time = 0;
			if(current_process->dt >= quantum)
					exe_time = quantum;
				else
					exe_time = current_process->dt;
			current_process->dt -= exe_time;
			en->exe_time = exe_time;
			en->proc = current_process;
			current_process->done = 0;
			queue_push(awaiting_process, en);
			if(heap_empty(ordered_process)) break;
			current_process = heap_min_element(ordered_process);
			printf("curr process %s\n", current_process->name);
			heap_pop(ordered_process);
		}

		while(!heap_empty(running_process) && running_time() > heap_min_time(running_process)){
			Process top = heap_min_element(running_process);
			printf("top process %s %.1f\n", top->name, top->dt);
			while(!top->done);
			printf("top process %s done  dt %.1f\n", top->name, top->dt);
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
				puts("before thread");
				pthread_create(top->thread,NULL,run_process,top);
				queue_push(awaiting_process, en);
				heap_pop(running_process);
			Process stop = ((Process)heap_min_element(running_process));
			queue_push(cpu_livre, &stop->cpu);
			}
			puts("sai do while 2");
		}

		while(!(awaiting_process->size == 0) && (running_process->size < ncores)){
			puts("while 3");
			if(queue_empty(cpu_livre))
				puts("FUUUUUUUUUUUUU");
			id = *((int*) head(cpu_livre));
			printf("cpu livre %d\n", id);
			queue_pop(cpu_livre);
			Process p = ((Exec_node) head(awaiting_process))->proc;
			double exe_time = ((Exec_node) head(awaiting_process))->exe_time;
			printf("unlock process %s\n", p->name);
			p->cpu = id; 
			pthread_mutex_unlock(p->mutex);
			heap_push(running_process,running_time() + exe_time, p);
			queue_pop(awaiting_process);
		}
	//	printf("running_process size %d awaiting process %d\n", running_process->size, awaiting_process->size);
	}
	puts("cabou");
	free(core);
	free(en);
}

int main(){
	FILE *trace = fopen("test.txt", "r"), *output = fopen("saida.txt", "w");
	if(trace == NULL)
		puts("File input open error");
	if(output == NULL)
		puts("File output open error");
	RR(trace, output, 5);
	fclose(trace);
	fclose(output);
}
