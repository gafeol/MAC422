#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define FL "trace.txt"	

int main(int argc, char *argv[]){
	int type = atoi(argv[1]);
	FILE *trace = fopen(argv[2], "r"), *output = fopen(argv[3], "w");
	switch (type) {
		case 1:
			//SJF(trace, output);
			break;
		case 2:
			//RR(trace, output);
			break;
		case 3:
			//P(trace, output);
			break;
		default:
			fprintf(stderr, "Escalonador invalido\n");
			break;
	} 

	fclose(trace); fclose(output);
	return 0;
}
