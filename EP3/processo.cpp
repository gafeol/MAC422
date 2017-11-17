#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

#define debug(args...) fprintf(stderr, args);

#include "global.h"
#include "processo.h"
#include "memory.h"
#include "best_fit.h"
#include "worst_fit.h"
#include "quick_fit.h"
#include "lru4.h"
#include "lista.h"

vector<processo> processos;

processo cria_processo(int t0, int tf, int b, string nome){
	return processo(t0, tf, b, nome);
}

void aloca_processo(int pro, int alg_aloc){
	clock_t clk = clock();

	// Seta pid do processo
	processos[pro].pid = pid_disp.front();
	printf("processo %d -> pid %d\n", pro, processos[pro].pid);
	pid_disp.pop();

	switch (alg_aloc) {
		case 1: 
			best_fit(pro);
		break;
		case 2:
			worst_fit(pro);
		break;
		case 3:
			quick_fit(pro);
		break;
	}
	tempo_busca += (double(clock() - clk))/CLOCKS_PER_SEC;
}

void remove_processo(int pro, int alg_subs, int alg_aloc){
	printf("remove processo %d\n", pro);
	if(alg_aloc < 3)
		lista_erase(L, pro);
	pid_disp.push(processos[pro].pid);
	remove_virtual(pro, alg_subs);
	
	if(alg_aloc == 3){
		proc.erase(pii(processos[pro].pos_virt, pro));

		int l = prv_pro(processos[pro].pos_virt);
		int r = nxt_pro(processos[pro].pos_virt);
		printf("l %d r %d\n", l, r);
		if(l != -1){
			int livre = processos[l].pos_virt + ceil(processos[l].b, tam_pag);
			if(pos[1].find(livre) != pos[1].end())
				pos[1].erase(livre);
			else if(pos[0].find(livre) != pos[0].end())
				pos[0].erase(livre);
		}
		int livre = processos[pro].pos_virt + ceil(processos[pro].b, tam_pag);
		if(pos[1].find(livre) != pos[1].end())
			pos[1].erase(livre);
		else if(pos[0].find(livre) != pos[0].end())
			pos[0].erase(livre);

		int ultl;
		if(r == -1)
			ultl = virt - 1;
		else
			ultl = processos[r].pos_virt - 1;
		int inil;
		if(l == -1)
			inil = 0;
		else
			inil = processos[l].pos_virt + ceil(processos[l].b, tam_pag);
		int taml = ultl - inil + 1;
		debug("inil %d ultl %d\n", inil, ultl);
		debug("retira processo %d, taml %d\n", pro, taml);
		if(taml >= val[1])
			pos[1].insert(inil);
		else if(taml >= val[0])
			pos[0].insert(inil);
	}
}

void acessa_pag(int p, int pos, int alg_subs){
	int pos_virt = processos[p].pos_virt + pos/tam_pag;

	qtd_aces[p][pos/tam_pag]--;

	if(MV[pos_virt].pos_fis == EMPTY){
		// tenta botar o maluco na memoria fisica 
		procura_fis(pos_virt, alg_subs);
	}
	R[MV[pos_virt].pos_fis] = 1;
	
	if(alg_subs == LRU4)
		atualiza_matriz4();
}
