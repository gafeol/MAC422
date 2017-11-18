#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

typedef pair<int, int> pii;

#define debug(args...) fprintf(stderr,args)

#include "global.h"
#include "sweep.h"
#include "processo.h"
#include "memory.h"
#include "lista.h"

int tmax;

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
	for(int a=0;a<nquad;a++)
		R[a] = 0;

	assert(alg_subs != -1 && "Algoritmo de substituicao de pagina nao escolhido");
	assert(alg_aloc != -1 && "Algoritmo de alocacao de memoria livre nao escolhido");

	if(alg_subs == FIFO){
		livre = (int*)malloc(nquad*sizeof(int));
		for(int a=0;a<nquad;a++)
			livre[a] = 0;
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

	L = lista_create();
	if(alg_aloc == 3){
		pos[1].clear();
		if(val[0] > val[1]) swap(val[0], val[1]);
		pos[0].clear();
		pos[1].insert(0);
	}

	//mkdir("/tmp", ACCESSPERMS);

	FILE *mem, *vir;

	mem = fopen("/tmp/ep3.mem", "wb+");
	assert(mem != NULL && "Erro na abertura de tmp/ep3.mem\n");

	vir = fopen("/tmp/ep3.vir", "wb+");
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
	//	debug("Evento %d %d\n", ev.tipo, ev.proc);
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
			case 6:
			{
				printf("Estado da memória no instante %d\n", ev.t);
				printf("  Memoria virtual (Bitmap e Estado)\n");
				char buffer;
				FILE *vir = fopen("/tmp/ep3.vir", "rb");
				for(int i=0;i<virt;i++){
					fread(&buffer, sizeof(char), 1, vir);
					if(buffer == EMPTY)
						printf("	0 -1\n");
					else
						printf("	1 %d\n", (int)buffer);
				}
				fclose(vir);
				printf("  Memoria física (Bitmap e Estado)\n");
				FILE *mem = fopen("/tmp/ep3.mem", "rb");
				for(int i=0;i<total;i++){
					fread(&buffer, sizeof(char), 1, mem);
					if(buffer == EMPTY)
						printf("	0 -1\n");
					else
						printf("	1 %d\n", (int)buffer);
				}
				fclose(mem);
				break;
			}
			default:
				break;
		}
	}

	free(R);

	if(alg_subs == FIFO)
		free(livre);

	if(alg_subs == LRU2 || alg_subs == LRU4){
		for(int i=0;i<nquad;i++)
			free(matriz_pag[i]);
		free(matriz_pag);
	}
	lista_delete(L);
}

void init(){
	tmax = 0;
	val[0] = val[1] = -1;
	page_fault = 0;
	tempo_busca = 0.;

	while(!pid_disp.empty())
		pid_disp.pop();

	// PARA TESTAR APAGAR DEPOIS NAO ESQUECERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	for(int a=0;a<100000;a++)
		pid_disp.push(1);
	//for(int a=0;a<128;a++)
//		pid_disp.push(a);

	qtd_aces.clear();
	nquad = 0;
	MV.clear();
	MF.clear();

	while(!fila_fis.empty())
		fila_fis.pop();

	freq.clear();

	pos[0].clear();
	pos[1].clear();

	proc.clear();

	processos.clear();
	eventos.clear();
}

void carrega(char* file){
	eventos.clear();
	processos.clear();

	FILE *trace;
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
			assert(pos < b);
			assert(t <= tf && t >= t0);
			adiciona_evento(t, 3, cnt, pos);
			tmax = max(tmax, t);
			p = strtok(NULL, " \n");
			qtd_aces[cnt][pos/tam_pag]++;
		}
		cnt++;
	}
	fclose(trace);

	// Eventos de atualizacao do bit R a cada unidade de tempo
	for(int tempo=0;tempo<=tmax+1;tempo++)
		adiciona_evento(tempo, 5,  0, 0);
}

int main(){
	tmax = 0;
	asserting = 1;
	int tipo_subs = -1, tipo_espaco = -1;
	char input[30], file[110];
	init();

	while(1){
		printf("[ep3]: ");
		scanf(" %s", input);
		if(strcmp(input, "sai") == 0)
			break;
		else if(strcmp(input, "carrega") == 0){
			scanf(" %s", file);
			carrega(file);
		}
		else if(strcmp(input, "substitui") == 0){
			scanf("%d", &tipo_subs);
		}
		else if(strcmp(input, "espaco") == 0){
			scanf("%d", &tipo_espaco);
		}
		else if(strcmp(input, "executa") == 0){
			scanf("%d", &dt);
			if(eventos.empty())
				carrega(file);
			/* OOOOOOOOOOOOOOOO NAO ESQUECE DE DESCOMENTAR NAO EM DEBUG
			for(int a=0;a*dt <= tmax;a++)
				adiciona_evento(a*dt, 6, 0, 0);
				*/
			roda(tipo_subs, tipo_espaco);
			printf("Tempo gasto buscando espaço livre na memória física:    %.10f\n", tempo_busca);
			printf("Numero de page faults: %d\n", page_fault);

			init();
			// Zerar o programa
		}
		else if(input[0] == 't'){
			//testa os parametros ja dados 30 vezes, para todas combinacoes espaco e substituicao
			FILE *tempo, *faults;
			tempo = fopen("./tempos.txt", "w");
			faults = fopen("./faults.txt", "w");
			for(int esp=1;esp<=3;esp++){
				int sub = 1;
				printf("Testando alg esp %d e alg sub %d\n", esp, sub);
				for(int test=0;test<30;test++){
					printf("Teste %d\n", test+1);
					if(eventos.empty())
						carrega(file);
					roda(sub, esp);
					printf("%.10f ", tempo_busca);
					printf("%d\n", page_fault);
					fprintf(tempo, "%.10f ", tempo_busca);
					fprintf(faults, "%d ", page_fault);
					init();

					fprintf(tempo, "\n");
					fprintf(faults, "\n");
				}
			}
			int esp = 1;
			for(int sub = 1;sub <= 4;sub++){
				printf("Testando alg esp %d e alg sub %d\n", esp, sub);
				for(int test=0;test<30;test++){
					printf("Teste %d\n", test+1);
					if(eventos.empty())
						carrega(file);
					roda(sub, esp);
					printf("%.10f ", tempo_busca);
					printf("%d\n", page_fault);
					fprintf(tempo, "%.10f ", tempo_busca);
					fprintf(faults, "%d ", page_fault);
					init();

					fprintf(tempo, "\n");
					fprintf(faults, "\n");
				}
			}
			fclose(tempo);
			fclose(faults);
		}
		else
			printf("Comando '%s' desconhecido\n", input);
	}
}
