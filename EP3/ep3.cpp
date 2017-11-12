#include <bits/stdc++.h>
using namespace std;

#include "processo.h"
#include "sweep.h"

void roda(FILE *trace, int dt, int subs, int espaco){
	FILE *mem, *vir;  
	mem = fopen("tmp/ep3.mem", "r+wb"); 
	vir = fopen("tmp/ep3.vir", "r+wb");
	int buffer = -1;
}

int main(){
	int dt;
	int tipo_subs = 0, tipo_espaco = 0;
	char input[30], file[110];
	FILE *trace;
	while(1){
		printf("[ep3]: ");
		scanf(" %s", input);
		printf("'%s'\n", input);
		if(strcmp(input, "sai") == 0)
			break;
		
		if(strcmp(input, "carrega") == 0){
			printf("carregou\n");
			scanf(" %s", file);
			trace = fopen(file, "r");
			int total, virt, ualoc, tam_pagina;
			fscanf(trace, "%d %d %d %d", &total, &virt, &ualoc, &tam_pagina);
			int t0, tf, b;
			char st[500010];
			int cnt = 0;
			while(fscanf(trace,"%d",&t0) != EOF) {
				cnt++;
				fscanf(trace, " %d%d", &tf, &b);
				fscanf(trace, " %s", st);
				string nome = st;
				fscanf(trace, "%[^\n]", st); 
				char *p = strtok(st, " \n");
				while(p != NULL) {
					int pos, t;
					sscanf(p, "%d", &pos);
					sscanf(p, "%d", &t);
					adiciona_evento(t, 3, cnt, pos);  
				}
			}	
			printf("pa\n");
		}
		else if(strcmp(input, "substitui") == 0){
			printf("substituiu\n");
			scanf("%d", &tipo_subs);
		}
		else if(strcmp(input, "espaco") == 0){
			scanf("%d", &tipo_espaco);
		}
		else if(strcmp(input, "executa") == 0){
			printf("executa\n");
			scanf("%d", &dt);
			roda(trace, dt, tipo_subs, tipo_espaco);
		}
		else{
			printf("Comando desconhecido\n");
			break;
		}
	}
}
