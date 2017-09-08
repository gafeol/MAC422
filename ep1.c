#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "print.h"
#include "p.h"
#include "rr.h"
#include "sjf.h"

int main(int argc, char *argv[]){
	lost_deadline = 0;
	context_change = 0;
	print_error = 0;

	int type = atoi(argv[1]);
	FILE *trace = fopen(argv[2], "r"), *output = fopen(argv[3], "w");
	if(argc > 4)
		print_error = atoi(argv[4]);


	int ncores = sysconf(_SC_NPROCESSORS_ONLN);
	switch (type) {
		case 1:
			SJF(trace, output, ncores);
			break;
		case 2:
			RR(trace, output, ncores);
			break;
		case 3:
			P(trace, output, ncores);
			break;
		default:
			fprintf(stderr, "Escalonador invalido\n");
			break;
	}

	printf("O escalonador desrespeitou %d deadlines\n", lost_deadline);
//	printf("%d\n", lost_deadline);
	fclose(trace); fclose(output);
	return 0;
}
