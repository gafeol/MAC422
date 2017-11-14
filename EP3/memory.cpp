#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr, args);

#include "memory.h"
#include "global.h"
#include "processo.h"

//#include "optimal.h"
#include "fifo.h"
#include "lru2.h"
//#include "lru4.h"


void seta_virtual(int pos_ini, int num_pag, int p){
	printf("seta virtual %d %d %d\n", pos_ini, num_pag, p);
	for(int a=pos_ini;a<pos_ini+num_pag;a++){
		MV[a] = mem_virt(p, EMPTY);
	}
	
	int sz = processos[p].b;
	printf("sz %d\n", sz);
	FILE *vir;
	vir = fopen("./tmp/ep3.vir", "r+b");
	assert(vir != NULL && "Erro na abertura de tmp/ep3.vir");

	fseek(vir, pos_ini*tam_pag*sizeof(char), SEEK_SET); 

	char buffer = processos[p].pid;
	for(int cnt=0;cnt < sz;cnt++){
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Erro na escrita de vir");
	}
	fclose(vir);

	getchar();
	getchar();
}

void remove_virtual(int p, int alg_subs){
	int pos_ini = processos[p].pos_virt;
	int sz = processos[p].b;
	int num_pag = ceil(sz, tam_pag);

	printf("rmv virtual %d %d %d\n", pos_ini, num_pag, p);
	for(int a=pos_ini;a<pos_ini+num_pag;a++){
		if(MV[a].pos_fis != EMPTY){
			debug("a %d pos fis %d inv %d\n", a, MV[a].pos_fis, MF[MV[a].pos_fis].pos_virt);
			assert(MF[MV[a].pos_fis].pos_virt == a);
			libera_fis(MV[a].pos_fis);
			MF[MV[a].pos_fis] = mem_fis();
			if(alg_subs == FIFO)
				livre[MV[a].pos_fis]++;
		}
		MV[a] = mem_virt();
		debug("seta MV %d pra empty\n", a);
	}
	
	FILE *vir;
	vir = fopen("./tmp/ep3.vir", "r+b");
	assert(vir != NULL && "Erro na abertura de tmp/ep3.vir");

	fseek(vir, pos_ini*tam_pag*sizeof(char), SEEK_SET); 

	char buffer = EMPTY;
	// NAO É SZ, TEM QUE LEVAR EM CONTA A UALOC
	debug("cara tem %d bytes, mas apago %d*%d\n", sz, num_pag, tam_pag);
	for(int cnt=0;cnt < num_pag*tam_pag;cnt++){
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Erro na escrita de vir");
	}
	fclose(vir);

	getchar();
	getchar();
}

void procura_fis(int pos_virt, int alg_subs){
	printf("aloca fis %d\n", pos_virt);
	for(int i=0;i < MF.size();i++){
		if(MF[i].pos_virt == EMPTY){
			MV[pos_virt].pos_fis = i;
			debug("liga virt %d - > mem %d\n", pos_virt, i);
			MF[i].pos_virt = pos_virt;
			debug("liga mem %d - > virt %d\n", i, pos_virt);
			MF[i].ind = MV[pos_virt].ind;
			
			if(alg_subs == FIFO)
				fila_fis.push(i);
			else if(alg_subs == LRU2)
				atualiza_matriz(i);

			aloca_fis(i, MF[i].ind);

			printf("nao usou alg subs\n");
			getchar();
			getchar();
			return ;
		}
	}

	alg_subs = LRU2;

	switch (alg_subs){
		/*case 1:
			optimal(pos_virt);
		break;*/
		case FIFO:
			fifo(pos_virt);
		break;
		case LRU2:
			lru2(pos_virt);
		break;
		/*case 4:
			lru4(pos_virt);
		break;*/
	}
}

void aloca_fis(int pos_fis, int p){
	modifica_fis(pos_fis, p);
}

void libera_fis(int pos_fis){
	modifica_fis(pos_fis, -1);
}

void modifica_fis(int pos_fis, int p){
	FILE *mem;
	mem = fopen("./tmp/ep3.mem", "r+b");
	char buffer;
	if(p == -1)
		buffer = EMPTY;
	else
		buffer = processos[p].pid;

	fseek(mem, pos_fis*tam_pag*sizeof(char), SEEK_SET);
	for(int a=0;a<tam_pag;a++){
		assert(fwrite(&buffer, sizeof(char), 1, mem) && "Erro na escrita de mem");
	}
	fclose(mem);
}

void substitui_pag(int pag, int pos_virt){
	MV[pos_virt].pos_fis = pag;
	MV[MF[pag].pos_virt].pos_fis = EMPTY;
	MF[pag].pos_virt = pos_virt;
	MF[pag].ind = MV[pos_virt].ind;

	FILE *mem;
	 
	mem = fopen("./tmp/ep3.mem", "r+b");

	char buffer  =  processos[MF[pag].ind].pid;
	debug("pag %d tam pag %d \n", pag, tam_pag);
	fseek(mem, pag*tam_pag*sizeof(char), SEEK_SET);
	for(int a=0;a<tam_pag;a++){
		assert(fwrite(&buffer, sizeof(char), 1, mem) && "Erro na escrita de mem");
	}
	fclose(mem);

	getchar();
	getchar();
}
