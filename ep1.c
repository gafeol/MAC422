#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


int main(int argc, char *argv[]){
	int type = atoi(argv[1]);
	FILE *trace = fopen(argv[2], "r"), *output = fopen(argv[3], "w");
	int ncores = sysconf(_SC_NPROCESSORS_ONLN);
	switch (type) {
		case 1:
			//SJF(trace, output, ncores);
			break;
		case 2:
			//RR(trace, output, ncores);
			break;
		case 3:
			//P(trace, output, ncores);
			break;
		default:
			fprintf(stderr, "Escalonador invalido\n");
			break;
	} 

	fclose(trace); fclose(output);
	return 0;
}
