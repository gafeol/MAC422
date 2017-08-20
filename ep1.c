// proto ep1 para testar o binario ./ep1 de ep1sh

#include<stdio.h>

int main(int argc, char *argv[]){
	printf("Hello World!\n");
	printf("%d parameters:\n", argc);
	for(int a=0;a<argc;a++){
		printf("%s\n", argv[a]);
	}
}
