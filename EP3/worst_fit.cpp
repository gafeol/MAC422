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

	if(asserting){
		// Fazendo alocacao com BitMap
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
		if(cnt >= sz_p && worst < cnt){
			worst = cnt;
			iworst = MV.size() - cnt;
		}
	}
	//Fazendo alocacao com Lista Ligada
	int pos = lista_push(L, p, 2);
	iworst = pos;

	if(asserting){
		assert(pos == iworst);
		assert(worst != -1);
	}
	else
		iworst = pos;

	processos[p].pos_virt = iworst;

	seta_virtual(iworst, sz_p, p);
}
