/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr, args);

#include "memory.h"
#include "global.h"
#include "processo.h"

#include "optimal.h"
#include "fifo.h"
#include "lru2.h"
#include "lru4.h"


void seta_virtual(int pos_ini, int num_pag, int p){
	for(int a=pos_ini;a<pos_ini+num_pag;a++){
		MV[a] = mem_virt(p, EMPTY);
	}

	int sz = processos[p].b;
	//printf("sz %d\n", sz);
	FILE *vir;
	vir = fopen("/tmp/ep3.vir", "r+b");
	assert(vir != NULL && "Erro na abertura de tmp/ep3.vir");

	fseek(vir, pos_ini*tam_pag*sizeof(char), SEEK_SET);

	char buffer = processos[p].pid;
	for(int cnt=0;cnt < ualoc*ceil(sz, ualoc);cnt++){
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Erro na escrita de vir");
	}
	fclose(vir);
}

void remove_virtual(int p, int alg_subs){
	int pos_ini = processos[p].pos_virt;
	int sz = processos[p].b;
	int num_pag = ceil(sz, tam_pag);

	for(int a=pos_ini;a<pos_ini+num_pag;a++){
		if(MV[a].pos_fis != EMPTY){
			assert(MF[MV[a].pos_fis].pos_virt == a);
			libera_fis(MV[a].pos_fis);
			MF[MV[a].pos_fis] = mem_fis();
			if(alg_subs == FIFO){
				livre[MV[a].pos_fis]++;
			}
		}
		MV[a] = mem_virt();
		assert(MV[a].ind == EMPTY);
	}

	FILE *vir;
	vir = fopen("/tmp/ep3.vir", "r+b");
	assert(vir != NULL && "Erro na abertura de tmp/ep3.vir");

	fseek(vir, pos_ini*tam_pag*sizeof(char), SEEK_SET);

	char buffer = EMPTY;

	for(int cnt=0;cnt < num_pag*tam_pag;cnt++){
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Erro na escrita de vir");
	}
	fclose(vir);
}

void procura_fis(int pos_virt, int alg_subs){
	assert(MV[pos_virt].ind != EMPTY);
	for(int i=0;i < MF.size();i++){
		if(MF[i].pos_virt == EMPTY){
			MV[pos_virt].pos_fis = i;
			MF[i].pos_virt = pos_virt;
			MF[i].ind = MV[pos_virt].ind;

			if(alg_subs == FIFO)
				fila_fis.push(i);
			else if(alg_subs == LRU2)
				atualiza_matriz(i);

			aloca_fis(i, MF[i].ind);
			return ;
		}
	}

	switch (alg_subs){
		case OPT:
			optimal(pos_virt);
		break;
		case FIFO:
			fifo(pos_virt);
		break;
		case LRU2:
			lru2(pos_virt);
		break;
		case LRU4:
			lru4(pos_virt);
		break;
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
	mem = fopen("/tmp/ep3.mem", "r+b");
	char buffer;
	if(p == -1)
		buffer = EMPTY;
	else
		buffer = processos[p].pid;

	fseek(mem, pos_fis*tam_pag*sizeof(char), SEEK_SET);
	for(int a=0;a<tam_pag;a++)
		assert(fwrite(&buffer, sizeof(char), 1, mem) == 1 && "Erro na escrita de mem");

	fclose(mem);
}

void substitui_pag(int pag, int pos_virt){
	MV[pos_virt].pos_fis = pag;
	assert(MF[pag].pos_virt != EMPTY);
	MV[MF[pag].pos_virt].pos_fis = EMPTY;
	MF[pag].pos_virt = pos_virt;
	MF[pag].ind = MV[pos_virt].ind;

	FILE *mem;

	mem = fopen("/tmp/ep3.mem", "r+b");
	char buffer  =  processos[MF[pag].ind].pid;
	fseek(mem, pag*tam_pag*sizeof(char), SEEK_SET);
	for(int a=0;a<tam_pag;a++)
		assert(fwrite(&buffer, sizeof(char), 1, mem) == 1 && "Erro na escrita de mem");
	fclose(mem);
}

void compacta()
{
	//compacta virtual
	int plivre = 0, pnlivre= 0;
	int num_pages = ceil(virt, tam_pag);
	int fis;
	FILE *vir;
	vir = fopen("/tmp/ep3.vir", "r+b");
	fseek(vir, 0, SEEK_SET);
	char buffer = EMPTY;
	for(int i = 0; i < num_pages*tam_pag; i++) {
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Nao foi possivel escrever no arquivo vir");
		fflush(vir);
	}
	fseek(vir, 0, SEEK_SET);
	while(plivre < num_pages && pnlivre < num_pages) {
		while(MV[pnlivre].ind == EMPTY && pnlivre < num_pages)
			pnlivre++;
		if(pnlivre >= num_pages)
			break;
		if(plivre != pnlivre) {
			fis = MV[pnlivre].pos_fis;
			MF[fis].pos_virt = plivre;
			MV[plivre].ind = MV[pnlivre].ind;
			MV[pnlivre].ind = EMPTY;
			MV[pnlivre].pos_fis = EMPTY;
			MV[plivre].pos_fis = fis;

			if(plivre == 0 || MV[plivre-1].ind != MV[plivre].ind)
				processos[MV[plivre].ind].pos_virt = plivre;
		}
		buffer = processos[MV[plivre].ind].pid;
		for(int i = 0; i < tam_pag; i++) {
			assert(fwrite(&buffer, sizeof(char), 1, vir) == 1  && "Nao foi possivel escrever no arquivo vir");
			fflush(vir);
		}
		plivre++; pnlivre++;
	}
	//compacta fisica
	plivre = 0, pnlivre= 0;
	int num_quadros = ceil(total, tam_pag);
	int virtl;
	FILE *mem;
	mem = fopen("/tmp/ep3.mem", "r+b");
	fseek(mem, 0, SEEK_SET);
	buffer = EMPTY;
	for(int i = 0; i < num_quadros*tam_pag; i++) {
		assert(fwrite(&buffer, sizeof(char), 1, mem) == 1 && "Nao foi possivel escrever no arquivo vir");
		fflush(mem);
	}
	fseek(mem, 0, SEEK_SET);
	while(plivre < num_quadros && pnlivre < num_quadros) {
		while(MF[pnlivre].ind == EMPTY && pnlivre < num_quadros)
			pnlivre++;
		if(pnlivre >= num_quadros)
			break;
		if(plivre != pnlivre) {
			virtl = MF[pnlivre].pos_virt;
			MV[virtl].pos_fis = plivre;
			MF[plivre].ind = MF[pnlivre].ind;
			MF[pnlivre].ind = EMPTY;
			MF[pnlivre].pos_virt = EMPTY;
			MF[plivre].pos_virt = virtl;
		}
		buffer = processos[MF[plivre].ind].pid;
		for(int i = 0; i < tam_pag; i++) {
			assert(fwrite(&buffer, sizeof(char), 1, mem) && "Nao foi possivel escrever no arquivo mem");
			fflush(mem);
		}
		plivre++; pnlivre++;
	}
	fclose(vir);
	fclose(mem);
}
