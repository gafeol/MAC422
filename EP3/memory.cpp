#include <bits/stdc++.h>
using namespace std;

#include "global.h"
#include "processo.h"

//#include "optimal.h"
#include "fifo.h"
//#include "lru2.h"
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

void remove_virtual(int pos_ini, int num_pag, int p){
	printf("rmv virtual %d %d %d\n", pos_ini, num_pag, p);
	for(int a=pos_ini;a<pos_ini+num_pag;a++){
		MV[a] = mem_virt();
	}
	
	int sz = processos[p].b;
	FILE *vir;
	vir = fopen("./tmp/ep3.vir", "r+b");
	assert(vir != NULL && "Erro na abertura de tmp/ep3.vir");

	fseek(vir, pos_ini*tam_pag*sizeof(char), SEEK_SET); 

	char buffer = EMPTY;
	for(int cnt=0;cnt < sz;cnt++){
		assert(fwrite(&buffer, sizeof(char), 1, vir) == 1 && "Erro na escrita de vir");
	}
	fclose(vir);

	getchar();
	getchar();
}

void aloca_fis(int pos_virt, int alg_subs){
	printf("aloca fis %d\n", pos_virt);
	for(int i=0;i < MF.size();i++){
		if(MF[i].pos_virt == EMPTY){
			MV[pos_virt].pos_fis = i;
			MF[i].pos_virt = pos_virt;
			MF[i].ind = MV[pos_virt].ind;

			fila_fis.push(i);

			FILE *mem;
			mem = fopen("./tmp/ep3.mem", "r+b");
			char buffer = 10; // processos[MF[i].ind].pid;
			fseek(mem, i*tam_pag*sizeof(char), SEEK_SET);
			assert(fwrite(&buffer, sizeof(char), 1, mem) && "Erro na escrita de mem");
			fclose(mem);

			printf("nao usou fifo\n");
			getchar();
			getchar();
			return ;
		}
	}

	alg_subs = 2;

	switch (alg_subs){
		/*case 1:
			optimal(pos_virt);
		break;*/
		case 2:
			fifo(pos_virt);
		break;
		/*case 3:
			lru2(pos_virt);
		break;
		case 4:
			lru4(pos_virt);
		break;*/
	}
}
