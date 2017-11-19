/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr,args)

#include "lru2.h"
#include "lru4.h"
#include "global.h"
#include "memory.h"

void lru4(int pos_virt){
	//debug("lru4 %d\n", pos_virt);

	// PAGE FAULT
	int mn = 0;
	for(int i=1;i<nquad;i++){
		if(cmp(i, mn))
			mn = i;
	}

	int pag = mn;

	substitui_pag(pag, pos_virt);
}

void atualiza_matriz4(){
	// Shift
	//debug("atualiza matriz4\n");
	for(int a=0;a<nquad;a++){
		for(int b=nquad-1;b>0;b--){
			matriz_pag[a][b] = matriz_pag[a][b-1];
		}
		matriz_pag[a][0] = R[a];
	}
	for(int a=0;a<nquad;a++){
		for(int b=0;b<nquad;b++){
			//debug("%d ", matriz_pag[a][b]);
		}
		//puts("");
	}
}
