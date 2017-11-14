#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

#define debug(args...) fprintf(stderr,args)

#include "global.h"
#include "sweep.h"
#include "processo.h"
#include "memory.h"

void roda(int alg_subs, int alg_aloc){

	nquad = ceil(total, tam_pag);

	R = (int*) malloc(nquad*sizeof(int));

	alg_subs = LRU2;

	if(alg_subs == FIFO){
		livre = (int*)malloc(nquad*sizeof(int));
		memset(livre, 0, sizeof(livre));
	}
	if(alg_subs == LRU2){
		int nquad = ceil(total, tam_pag);
		matriz_pag = (bool **) malloc(nquad*sizeof(bool*));
		for(int i=0;i<nquad;i++){
			matriz_pag[i] = (bool *) malloc(nquad*sizeof(bool));
			for(int j=0;j<nquad;j++){
				matriz_pag[i][j] = 0;
			}
		}
	}

	mkdir("./tmp", ACCESSPERMS);

	FILE *mem, *vir;

	mem = fopen("./tmp/ep3.mem", "wb+"); 
	assert(mem != NULL && "Erro na abertura de tmp/ep3.mem\n");

	vir = fopen("./tmp/ep3.vir", "wb+");
	assert(vir != NULL && "Erro na abertura de tmp/ep3.vir\n");
	
	char buffer = EMPTY;

	fseek(mem, 0, SEEK_SET);
	fseek(vir, 0, SEEK_SET);

	for(int i=0;i < total;i++)
		assert(fwrite(&buffer, sizeof(char), 1, mem) == 1 && "Erro na escrita do arquivo ep3.mem"); 

	for(int i=0;i < ceil(total, tam_pag);i++)
		MF.push_back(mem_fis());

	for(int i=0;i < virt;i++)
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Erro na escrita do arquivo ep3.vir");

	for(int i=0;i < ceil(virt, tam_pag);i++)
		MV.push_back(mem_virt());

	fclose(mem);
	fclose(vir);

	while(!eventos.empty()){
		evento ev = prox_evento(); 
		printf("evento %d %d\n", ev.tipo, ev.proc);
		int proc = ev.proc;
		int pos = ev.pos;
		switch (ev.tipo){
			case 1:
				aloca_processo(proc, alg_aloc);
				break;
			case 2:
				remove_processo(proc, alg_subs);
				break;
			case 3:
				acessa_pag(proc, pos, alg_subs);	
				break;
/*			case 4:
				compacta();
				break;
*/		
			default:
				break;
		}
	}

	if(alg_subs == FIFO)
		free(livre);
	else if(alg_subs == LRU2)
		free(matriz_pag);
}

int main(){
	int tipo_subs = 0, tipo_espaco = 0;
	char input[30], file[110];
	FILE *trace;
	
	for(int a=0;a<128;a++){
		pid_disp.push(a);
	}
	while(1){
		printf("[ep3]: ");
		scanf(" %s", input);
		if(strcmp(input, "sai") == 0)
			break;
		
		if(strcmp(input, "carrega") == 0){
			printf("carregou\n");
			scanf(" %s", file);
			trace = fopen(file, "r");
			fscanf(trace, "%d %d %d %d", &total, &virt, &ualoc, &tam_pag);
			int t0, tf, b;
			char st[500010];
			int cnt = 0;
			while(fscanf(trace,"%d",&t0) != EOF) {
				fscanf(trace, " %s", st);
				if(strcmp(st, "COMPACTAR") == 0){
					adiciona_evento(t0, 4, 0, 0);
					continue;
				}
				else
					tf = atoi(st);
				fscanf(trace, " %d", &b);
				fscanf(trace, " %s", st);
				string nome = st;
				adiciona_evento(t0, 1, cnt, 0);
				adiciona_evento(tf, 2, cnt, 0);
	
				processo novo  = cria_processo(t0, tf, b, nome); 
				processos.push_back(novo);

				fscanf(trace, "%[^\n]", st); 
				char *p = strtok(st, " \n");
				while(p != NULL) {
					int pos, t;
					sscanf(p, "%d", &pos);
					p = strtok(NULL, " \n");
					sscanf(p, "%d", &t);
					adiciona_evento(t, 3, cnt, pos); 
					p = strtok(NULL, " \n"); 
				}
				cnt++;
			}	
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
			roda(tipo_subs, tipo_espaco);
		}
		else{
			printf("Comando desconhecido\n");
			break;
		}
	}
}
