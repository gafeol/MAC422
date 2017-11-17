#include <bits/stdc++.h>
using namespace std;
#define debug(args...) fprintf(stderr,args)

typedef pair<int, int> pii;

#include "quick_fit.h"
#include "best_fit.h"
#include "global.h"
#include "memory.h"
#include "processo.h"



int prv_pro(int ini){
	if(!proc.empty() && proc.lower_bound(pii(ini, -1)) != proc.begin())
		return (--proc.lower_bound(pii(ini, -1)))->second;
	return -1;
}

int nxt_pro(int ini){
	if(!proc.empty() && proc.upper_bound(pii(ini, INT_MAX)) != proc.end())
		return (proc.upper_bound(pii(ini , INT_MAX)))->second;
	return -1;
}

void atualiza_set(int p){
	int esp_livre = 0;
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);
	int ini = processos[p].pos_virt;
	int nxt = nxt_pro(ini);
	if(nxt != -1)
		esp_livre = processos[nxt].pos_virt - 1 - (ini + sz_p) + 1;
	else
		esp_livre = virt - 1 - (ini + sz_p) + 1;

	if(esp_livre >= val[1])
		pos[1].insert(ini+sz_p);
	else if(esp_livre >= val[0])
		pos[0].insert(ini+sz_p);
}

void print(int i){
	debug("pos[%d] : ", i);
	for(set<int>::iterator it = pos[i].begin(); it != pos[i].end();it++){
		debug("%d ", *it);	
	}
	debug("\n");
}

void quick_fit(int p){
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);

	//debug("val0 %d val1 %d\n", val[0], val[1]);
	//print(0);
	//print(1);

	if(sz_p > val[1]){
		best_fit(p);
		int ini = processos[p].pos_virt;
		if(pos[1].find(ini) != pos[1].end())
			pos[1].erase(ini);
		else if(pos[0].find(ini) != pos[0].end())
			pos[0].erase(ini);
		atualiza_set(p);
	}
	else if(sz_p > val[0]){
		int ini = *pos[1].begin();
		pos[1].erase(ini);
		processos[p].pos_virt = ini;
		seta_virtual(ini, sz, p);
		atualiza_set(p);
	}
	else{
		int ini;
		if(!pos[0].empty()){
			ini = *pos[0].begin();
			pos[0].erase(ini);
		}
		else{
			ini = *pos[1].begin();
			pos[1].erase(ini);
		}
		processos[p].pos_virt = ini;
		seta_virtual(ini, sz, p);
		atualiza_set(p);
	}

	int l = prv_pro(processos[p].pos_virt);
	int r = nxt_pro(processos[p].pos_virt);
	//debug("cara %d pos virt %d esq %d dir %d\n", p, processos[p].pos_virt, l, r);

	proc.insert(pii(processos[p].pos_virt, p));
}
