#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr, args);

#include "worst_fit.h"
#include "global.h"
#include "processo.h"
#include "memory.h"
#include "lista.h"

void worst_fit(int p){
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);
	int worst = -1, iworst;
	int cnt = 0;

	// Fazendo alocacao com BitMap
	printf("cnt %d sz p %d\n", cnt, sz_p);
	for(int i=0;i<MV.size();i++){
		if(MV[i].ind != EMPTY){
			if(cnt >= sz_p && worst < cnt){
				worst = cnt;
				iworst = i - cnt;
			}
			cnt = 0;
		}
		else
			cnt++;
	}
	printf("cnt %d sz p %d worst %d worsti %d\n", cnt, sz_p, worst, iworst);

	if(cnt >= sz_p && worst < cnt){
		worst = cnt;
		iworst = MV.size() - cnt;
	}

	//Fazendo alocacao com Lista Ligada
	int pos = lista_push(L, p, 2);

	assert(pos == iworst);
	debug("BATEU! AAAAAAAAAAAAAAAAAAAAA %d = %d\n", pos, iworst);

	processos[p].pos_virt = iworst;

	printf("cnt %d sz p %d worst %d worsti %d\n", cnt, sz_p, worst, iworst);

	printf("seta virtual %d %d %d\n", iworst, sz_p, p);

	seta_virtual(iworst, sz, p);
}
