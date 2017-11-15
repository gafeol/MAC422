#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

typedef pair<int, int> pii;

#define debug(args...) fprintf(stderr,args)

#include "global.h"
#include "sweep.h"
#include "processo.h"
#include "memory.h"

void verifica_freq(int npag){
	if(val[0] == npag || val[1] == npag) return;
	if(val[0] != -1 && val[1] != -1){
		if(freq[val[0]] < freq[val[1]])
			swap(val[0], val[1]); 
			// Garante que val 0 sera o npag mais frequente
	}
	if(freq[npag] > freq[val[0]] || val[0] == -1){
		val[1] = val[0];
		val[0] = npag;
	}
	else if(freq[npag] > freq[val[1]] || val[1] == -1){
		val[1] = npag;
	}
}

void roda(int alg_subs, int alg_aloc){

	nquad = ceil(total, tam_pag);

	R = (int*) malloc(nquad*sizeof(int));

	alg_subs = OPT;
	alg_aloc = 3;

	if(alg_subs == FIFO){
		livre = (int*)malloc(nquad*sizeof(int));
		memset(livre, 0, sizeof(livre));
	}
	if(alg_subs == LRU2 || alg_subs == LRU4){
		matriz_pag = (bool **) malloc(nquad*sizeof(bool*));
		for(int i=0;i<nquad;i++){
			matriz_pag[i] = (bool *) malloc(nquad*sizeof(bool));
			for(int j=0;j<nquad;j++){
				matriz_pag[i][j] = 0;
			}
		}
	}
	if(alg_aloc == 3){
		if(val[0] > val[1]) swap(val[0], val[1]);		
		int npag = ceil(virt, tam_pag);
		pos[1].insert(0);
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
				remove_processo(proc, alg_subs, alg_aloc);
				break;
			case 3:
				acessa_pag(proc, pos, alg_subs);	
				break;
			case 4:
				compacta();
				break;
			case 5:
				for(int a=0;a<nquad;a++)
					R[a] = 0;
				break;
			default:
				break;
		}
	}

	if(alg_subs == FIFO)
		free(livre);
	else if(alg_subs == LRU2 || alg_subs == LRU4)
		free(matriz_pag);
}

int main(){
	val[0] = val[1] = -1;
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
			int tmax = 0;
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
				tmax = max(tmax, t0);
				tmax = max(tmax, tf);
				fscanf(trace, " %d", &b);

				int npag = ceil(b, tam_pag);
				freq[npag]++;
				verifica_freq(npag);

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
					tmax = max(tmax, t);
					p = strtok(NULL, " \n"); 
					
					debug("qtd_aces[%d][%d] ++\n", cnt, pos/tam_pag);
					qtd_aces[cnt][pos/tam_pag]++;
				}
				cnt++;
			}	
			fclose(trace);	

			// Eventos de atualizacao do bit R a cada unidade de tempo
			for(int tempo=0;tempo<=tmax+1;tempo++)
				adiciona_evento(tempo, 5,  0, 0);
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
