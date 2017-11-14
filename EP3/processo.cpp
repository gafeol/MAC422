#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr, args);

#include "global.h"
#include "processo.h"
#include "memory.h"
#include "best_fit.h"
#include "worst_fit.h"
//#include "quick_fit.h"
#include "lru4.h"

vector<processo> processos;

processo cria_processo(int t0, int tf, int b, string nome){
	return processo(t0, tf, b, nome);
}

void aloca_processo(int pro, int alg_aloc){

	alg_aloc = 2; /* FORÇANDO WORST FIT */

	// Seta pid do processo
	processos[pro].pid = pid_disp.front();
	pid_disp.pop();

	switch (alg_aloc) {
		case 1: 
			best_fit(pro);
		break;
		case 2:
			worst_fit(pro);
		break;
/*		case 3:
			quick_fit(pro);
		break;*/
	}
}

void remove_processo(int pro, int alg_subs){
	printf("remove processo %d\n", pro);
	pid_disp.push(processos[pro].pid);
	remove_virtual(pro, alg_subs);
}

void acessa_pag(int p, int pos, int alg_subs){
	int pos_virt = processos[p].pos_virt + pos/tam_pag;
	debug("acessa pag %d %d pos fis %d\n", p, pos, MV[pos_virt].pos_fis);
	if(MV[pos_virt].pos_fis == EMPTY){
		debug("procura fis\n");
		// tenta botar o maluco na memoria fisica 
		procura_fis(pos_virt, alg_subs);
	}
	R[MV[pos_virt].pos_fis] = 1;
	
	if(alg_subs == LRU4)
		atualiza_matriz4();
}
