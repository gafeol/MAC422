#include <stdio.h>
#include <time.h>
#include <stdint.h>
#define MST (-7)
#define BRT (-3)
#define UTC (0)
#define CCT (+8)

int main ()
{
	time_t rawtime;
	struct tm * ptm;

	time ( &rawtime );

	ptm = gmtime ( &rawtime );

	printf ("Beijing (China) :     %2d:%02d\n", (ptm->tm_hour+CCT)%24, ptm->tm_min);

	time_t result = time(NULL);

	if(result != -1){
		printf("%s", asctime(localtime(&result)));
	}
}
