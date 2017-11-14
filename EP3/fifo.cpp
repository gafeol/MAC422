#include <bits/stdc++.h>
using namespace std;

#include "fifo.h"
#include "global.h"
#include "processo.h"

#define debug(args...) fprintf(stderr,args)

void fifo(int pos_virt){

	// PAGE FAULT
	while(!fila_fis.empty() && livre[fila_fis.front()]){
		livre[fila_fis.front()]--;
		fila_fis.pop();
	}
	assert(!fila_fis.empty());
	int pag = fila_fis.front();
	fila_fis.pop();
	fila_fis.push(pag);
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
