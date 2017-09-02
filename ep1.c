#include<stdio.h>
#include<pthread.h>

#define FL "trace.txt"	

int main(int argc, char *argv[]){
	FILE *fp;
	fopen(FL, 
	printf("Hello World!\n");
	printf("%d parameters:\n", argc);
	for(int a=0;a<argc;a++){
		printf("%s\n", argv[a]);
	}
}
