#include "process.h"
#include "queue.h"
#include "heap.h"

SJF(FILE* input, FILE* output){
	char *line = NULL;
	Heap *ordered_process = heap_create();
	while(getline(&line, &len, input)){
		Process *p = process_line(line);
		heap_push(ordered_process, p->t0, p);
	}
	float time = 0;
	Heap *next_process = heap_create();
	while(ordered_process->size != 0 || next_process->size != 0){
		while(ordered_process->size > 0 && time > heap_min_time(ordered_process)){
			Process *top = heap_min_element(ordered_process);
			heap_push(next_process, top->dt, top);
		}
	}
}


