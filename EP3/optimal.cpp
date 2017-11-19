/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr,args)

#include "global.h"
#include "optimal.h"
#include "processo.h"
#include "memory.h"

void optimal(int pos_virt){
	// PAGE FAULT
   	//debug("optimal, quero alocar para a pos virt %d\n", pos_virt);

	int pag;

	int mn = INT_MAX, imn;
	for(int i=0;i < MF.size();i++){
		int p = MF[i].ind;
		int pag_rel = (MF[i].pos_virt - processos[p].pos_virt);
		//debug("	p %d pag rel %d: acessada %d\n", p, pag_rel, qtd_aces[p][pag_rel]);
		if(mn > qtd_aces[p][pag_rel]){
			mn = qtd_aces[p][pag_rel];
			imn = i;
		}
	}
	pag = imn;
	//debug("retira pag %d\n", pag);

	substitui_pag(pag, pos_virt);
}
