/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;

#define debug(args...) fprintf(stderr,args)

#include "lru2.h"
#include "global.h"
#include "memory.h"

bool cmp(int a, int b){
	int nquad = ceil(total, tam_pag);
	for(int i=0;i<nquad;i++){
		if(matriz_pag[a][i] != matriz_pag[b][i])
			return (matriz_pag[a][i] < matriz_pag[b][i]);
	}
	return 0;
}

void lru2(int pos_virt){
	int mn = 0;
	for(int i=1;i<nquad;i++){
		if(cmp(i, mn))
			mn = i;
	}
	int pag = mn;

	substitui_pag(pag, pos_virt);

	atualiza_matriz(pag);

}

void atualiza_matriz(int pag){
	for(int j=0;j<nquad;j++)
		matriz_pag[pag][j] = 1;
	for(int i=0;i<nquad;i++)
		matriz_pag[i][pag] = 0;
}
