#include <stdio.h>
#include <stdlib.h>

int *s;

int main (int argc, char* argv[]){
	int n = atoi(argv[1]);
	printf("argc %d n %d\n", argc, n);
	s = malloc(n*sizeof(int));
	for(int a=0;a<n;a++){
		s[a] = a;
		printf("%d\n", s[a]);
	}
	free(s);
}
