#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr, args);

#include "best_fit.h"
#include "global.h"
#include "processo.h"
#include "lista.h"
#include "memory.h"

void best_fit(int p){
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);
	int best = INT_MAX, ibest;
	int cnt = 0;

	//Fazendo alocacao com BitMap
	printf("cnt %d sz p %d\n", cnt, sz_p);
	for(int i=0;i<MV.size();i++){
		if(MV[i].ind != EMPTY){
			if(cnt >= sz_p && best > cnt){
				best = cnt;
				ibest = i - cnt;
			}
			cnt = 0;
		}
		else
			cnt++;
	}
	printf("cnt %d sz p %d best %d besti %d\n", cnt, sz_p, best, ibest);

	if(cnt >= sz_p && best > cnt){
		best = cnt;
		ibest = MV.size() - cnt;
	}

	//Alteracoes com Lista Ligada
	int pos = lista_push(L, p, 1);
	ibest = pos;

	assert(ibest == pos);

	processos[p].pos_virt = ibest;

	assert(best != INT_MAX);
	seta_virtual(ibest, sz_p, p);
}
