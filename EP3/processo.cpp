#include <bits/stdc++.h>
using namespace std;

#include "global.h"
#include "processo.h"
#include "memory.h"
#include "best_fit.h"
#include "worst_fit.h"
//#include "quick_fit.h"

vector<processo> processos;

processo cria_processo(int t0, int tf, int b, string nome){
	return processo(t0, tf, b, nome);
}

void aloca_processo(int pro, int alg_aloc){

	alg_aloc = 2; /* FORÇANDO WORST FIT */

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

void remove_processo(int pro){
	int sz = processos[pro].b;
	int sz_p = ceil(sz, tam_pag);
	printf("remove processo %d\n", pro);
	remove_virtual(processos[pro].pos_virt, sz_p, pro);
}

void acessa_pag(int p, int pos, int alg_subs){
	int pos_virt = processos[p].pos_virt + pos/tam_pag;
	if(MV[pos_virt].pos_fis == EMPTY){
		// tenta botar o maluco na memoria fisica 
		aloca_fis(pos_virt, alg_subs);
	}
}
