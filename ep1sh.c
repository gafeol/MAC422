#include <stdio.h>
#include <time.h>
#include <stdint.h>

int main(void){
	time_t result = time(NULL);
	if(result != -1)
		printf("%s", asctime(gmtime(&result)));
}
