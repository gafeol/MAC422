#include <stdlib.h>
#include <sys/time.h>
#include "calctime.h"

double sec(timev t){
	return t.tv_sec + t.tv_usec/1000000.;
}

double running_time(){
	timev act;
	gettimeofday(&act, NULL);
	printf("running time %.3f - %.3f\n", sec(act), sec(start_time));
	return sec(act) - sec(start_time); 
}
