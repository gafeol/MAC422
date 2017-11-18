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
	//printf("processo %d -> pid %d\n", pro, processos[pro].pid);
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
	lista_erase(L, pro);

	pid_disp.push(processos[pro].pid);
	remove_virtual(pro, alg_subs);

	if(alg_aloc == 3){
		proc.erase(pii(processos[pro].pos_virt, pro));

		int l = prv_pro(processos[pro].pos_virt);
		int r = nxt_pro(processos[pro].pos_virt);// + ceil(processos[pro].b, tam_pag));

		//APAGAR VERIFICACAO
		int ll = slowprv_pro(processos[pro].pos_virt);
		int rr = slownxt_pro(processos[pro].pos_virt + ceil(processos[pro].b, tam_pag));
		assert(l == ll);
		assert(r == rr);
		printf("l %d r %d\n", l, r);


		int livre;
		if(l == -1)
			livre = 0;
		if(l != -1)
			livre = processos[l].pos_virt + ceil(processos[l].b, tam_pag);

		if(pos[1].find(livre) != pos[1].end())
			pos[1].erase(livre);
		if(pos[0].find(livre) != pos[0].end())
			pos[0].erase(livre);

		livre = processos[pro].pos_virt + ceil(processos[pro].b, tam_pag);

		if(pos[1].find(livre) != pos[1].end())
			pos[1].erase(livre);
		if(pos[0].find(livre) != pos[0].end())
			pos[0].erase(livre);

		int ultl;
		if(r == -1)
			ultl = ceil(virt, tam_pag) - 1;
		else
			ultl = processos[r].pos_virt - 1;
		int inil;
		if(l == -1)
			inil = 0;
		else
			inil = processos[l].pos_virt + ceil(processos[l].b, tam_pag);

		assert(inil -1 < 0 || MV[inil-1].ind != EMPTY);
		assert(ultl+1 >= ceil(virt, tam_pag) || MV[ultl+1].ind != EMPTY);

		for(int i=inil;i <= ultl;i++){
			assert(pos[1].find(i) == pos[1].end());
			assert(pos[0].find(i) == pos[0].end());
		}

		int taml = ultl - inil + 1;
		//debug("inil %d ultl %d\n", inil, ultl);
		//debug("retira processo %d, taml %d\n", pro, taml);
		if(taml >= val[1])
			pos[1].insert(inil);
		else if(taml >= val[0])
			pos[0].insert(inil);
	}
}

void acessa_pag(int p, int pos, int alg_subs){
	int pos_virt = processos[p].pos_virt + pos/tam_pag;
	if(MV[pos_virt].ind == EMPTY){
		fprintf(stderr, "AAAAAAAAAA\n");
		fprintf(stderr, "p %d pos %d\n", p, pos);
	}
	assert(MV[pos_virt].ind != EMPTY);
	qtd_aces[p][pos/tam_pag]--;

	if(MV[pos_virt].pos_fis == EMPTY){
		// tenta botar o maluco na memoria fisica
		page_fault++;
		procura_fis(pos_virt, alg_subs);
	}
	R[MV[pos_virt].pos_fis] = 1;

	if(alg_subs == LRU4)
		atualiza_matriz4();
}
