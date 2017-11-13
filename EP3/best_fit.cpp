#include <bits/stdc++.h>
using namespace std;

#include "best_fit.h"
#include "global.h"
#include "processo.h"
#include "memory.h"

void best_fit(int p){
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);
	int best = INT_MAX, ibest;
	int cnt = 0;
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

	processos[p].pos_virt = ibest;

	printf("cnt %d sz p %d best %d besti %d\n", cnt, sz_p, best, ibest);

	printf("seta virtual %d %d %d\n", ibest, sz_p, p);

	seta_virtual(ibest, sz, p);
}
