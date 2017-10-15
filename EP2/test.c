#include <stdio.h>
#include <stdlib.h>

int *s;

int main (){
	s = malloc(50*sizeof(int));
	for(int a=0;a<50;a++){
		s[a] = a;
		printf("%d\n", s[a]);
	}
	free(s);
}
