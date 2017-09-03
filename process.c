#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "process.h"

#define MAX_SZ 100

Process *process_create(float t0, float dt, float deadline, char *name){
	Process *new_process = malloc(sizeof(Process));
	new_process->t0 = t0;
	new_process->dt = dt;
	new_process->deadline = deadline;
	new_process->name = malloc(MAX_SZ*sizeof(char));
	strcpy(new_process->name, name);
	new_process->priority = 0;
	new_process->elapsed = 0;
	new_process->mutex = malloc(sizeof(pthread_mutex_t));
	new_process->thread = malloc(sizeof(pthread_t));

	return new_process;
}

Process *process_line(char *line){
	char **token = malloc(MAX_SZ*sizeof(char*));
	int cnt = 0;
	char *st = strtok(line, " ");
	while(st != NULL){
		token[cnt++] = st;
		st = strtok(NULL, " ");
	}
	Process *p = process_create(atof(token[0]), atof(token[1]), atof(token[2]), token[3]);
	free(token);

	return p;
}


void process_delete(Process *p){
	free(p->name);
	free(p->thread);
	pthread_mutex_destroy(p->mutex);
	free(p->mutex);
	free(p);
}


#define MAXL 100

int main(int argc, char *argv[]){
	FILE *fp;
	fp = fopen(argv[1], "r");	
	char *line = malloc(sizeof(char)*MAXL);
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, fp)) != -1) {
		printf("%s", line);
		Process *p = process_line(line);
		printf("t0 %f dt %f deadline %f name %s\n", p->t0, p->dt, p->deadline, p->name);
		process_delete(p);
	}
	fclose(fp);
	free(line);
}
