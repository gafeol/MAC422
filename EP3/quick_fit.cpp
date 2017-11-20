/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;
#define debug(args...) fprintf(stderr,args)

typedef pair<int, int> pii;

#include "quick_fit.h"
#include "best_fit.h"
#include "global.h"
#include "memory.h"
#include "processo.h"
#include "lista.h"

int prv_pro(int ini){
	if(!proc.empty() && proc.lower_bound(pii(ini, -1)) != proc.begin())
		return (--proc.lower_bound(pii(ini, -1)))->second;
	return -1;
}

int slowprv_pro(int ini){
	int i = ini-1;
	while(MV[i].ind == EMPTY && i >= 0)
		i--;
	if(i != -1)
		return MV[i].ind;
	return -1;

}

int nxt_pro(int ini){
	if(!proc.empty() && proc.upper_bound(pii(ini, INT_MAX)) != proc.end())
		return (proc.upper_bound(pii(ini , INT_MAX)))->second;
	return -1;
}

int slownxt_pro(int ini){
	int i = ini;
	while(MV[i].ind == EMPTY && i < ceil(virt, tam_pag))
		i++;
	if(i != ceil(virt, tam_pag))
		return MV[i].ind;
	return -1;
}

void atualiza_set(int p){
	int esp_livre = 0;
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);
	int ini = processos[p].pos_virt;

	int confere = 0;
	if(asserting){
		for(int i=ini;i<ini+sz_p;i++)
			assert(MV[i].ind == p);
		for(int i = ini-1;i>=0;i--)
			assert(MV[i].ind != p);
		for(int i = ini+sz_p;i < ceil(virt, tam_pag);i++)
			assert(MV[i].ind != p);
		int i;
		for(i=ini+sz_p;i < ceil(virt, tam_pag) && MV[i].ind == EMPTY;i++)
			confere++;
	}

	int prox = nxt_pro(ini);
	if(asserting){
		int pprox = slownxt_pro(ini+sz_p);
		assert(prox == pprox);
	}
	if(prox == -1)
		esp_livre = ceil(virt, tam_pag)-1;
	else
		esp_livre = processos[prox].pos_virt - 1;

	esp_livre = esp_livre - (ini+sz_p) + 1;

	if(asserting)
		assert(confere == esp_livre);

	if(esp_livre >= val[1]){
		assert(MV[ini+sz_p].ind == EMPTY);
		pos[1].insert(ini+sz_p);
	}
	else if(esp_livre >= val[0]){
		assert(MV[ini+sz_p].ind == EMPTY);
		pos[0].insert(ini+sz_p);
	}


	if(asserting){
		confere = 0;
		if(ini >= 0)
			assert(MV[ini-1].ind != EMPTY);
		int i;
		for(i = ini-1;i >= 0 && MV[i].ind == EMPTY;i--)
			confere++;

		if(i >= 0)
			assert(MV[i].ind != p);

		assert(confere == 0);

		esp_livre = confere;
		if(esp_livre >= val[1]){
			assert(MV[i].ind == EMPTY);
			pos[1].insert(i);
		}
		else if(esp_livre >= val[0]){
			assert(MV[i].ind == EMPTY);
			pos[0].insert(i);
		}
	}
}

void quick_fit(int p){
	int sz = processos[p].b;
	int sz_p = ceil(sz, tam_pag);

	if(sz_p > val[1]){
		best_fit(p);
		int ini = processos[p].pos_virt;
		if(pos[1].find(ini) != pos[1].end())
			pos[1].erase(ini);
		if(pos[0].find(ini) != pos[0].end())
			pos[0].erase(ini);
		atualiza_set(p);
	}
	else if(sz_p > val[0]){
		assert(!pos[1].empty());
		int ini = *pos[1].begin();
		pos[1].erase(ini);
		if(asserting){
			for(int i=ini;i<ini+sz_p;i++){
				assert(MV[i].ind == EMPTY);
				if(pos[1].find(i) != pos[1].end())
					assert(false);
				if(pos[0].find(i) != pos[0].end())
					assert(false);
			}
		}
		processos[p].pos_virt = ini;
		assert(MV[ini].ind == EMPTY);
		seta_virtual(ini, sz_p, p);
		atualiza_set(p);
		lista_insert(L, p, processos[p].pos_virt);
	}
	else{
		int ini;
		if(!pos[0].empty()){
			ini = *pos[0].begin();
			pos[0].erase(ini);
			if(asserting){
				for(int i=ini;i<ini+sz_p;i++){
					assert(MV[i].ind == EMPTY);
					if(pos[1].find(i) != pos[1].end())
						assert(false);
					if(pos[0].find(i) != pos[0].end())
						assert(false);
				}
			}
		}
		else{
			assert(!pos[1].empty());
			ini = *pos[1].begin();
			pos[1].erase(ini);
			if(asserting){
				for(int i=ini;i<ini+sz_p;i++){
					if(pos[1].find(i) != pos[1].end())
						assert(false);
					if(pos[0].find(i) != pos[0].end())
						assert(false);
				}
			}
		}
		processos[p].pos_virt = ini;
		assert(MV[ini].ind == EMPTY);
		seta_virtual(ini, sz_p, p);
		atualiza_set(p);
		lista_insert(L, p, processos[p].pos_virt);
	}

	proc.insert(pii(processos[p].pos_virt, p));
}
