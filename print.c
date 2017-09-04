#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "print.h"

void print_trace(Process p){
	readline++;
	if(print_error) fprintf(stderr, "Processo %s chegou no sistema na linha %d do arquivo de trace.\n", p->name, readline);
}

void print_cpu_usage(Process p, int cpu){
	if(print_error) fprintf(stderr, "Processo %s comecou a usar a cpu de indice %d\n", p-> name, cpu);
}

void print_cpu_liberation(Process p, int cpu){
	if(print_error) fprintf(stderr, "Processo %s liberou a cpu de indice %d\n", p->name, cpu);
}
void print_output(Process p){
	printline++;
	if(print_error) fprintf(stderr, "Processo %s finalizou sua execução e foi impresso na linha %d do output\n", p->name, printline);
}
void print_context_change(){
	if(print_error) fprintf(stderr, "No total, o escalonador mudou de contexto %d vezes\n", context_change);
}

/*
int main(){
	context_change = 1;
	Process p = malloc(sizeof(Process));
	p->name = "processo";
	print_trace(p, 1);
	print_error = 1;
	print_trace(p, 1);
	print_cpu_usage(p, 2);
	print_cpu_liberation(p, 2);
	print_context_change();
	print_output(p, 3);
}
*/
