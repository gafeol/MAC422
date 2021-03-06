/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#ifndef SWEEP_H
#define SWEEP_H

#include <bits/stdc++.h>
using namespace std;

/* Ordem de desempate dos eventos
	Bit R
	t0 de pag
	acesso de pag
	compactacao
	tf de pag
	Impressao do estado das memorias
*/

struct evento{
	int tipo;
	int t;

	int proc;
	int pos;

	evento (int ttipo, int tt, int pproc, int ppos){
		tipo = ttipo;
		t = tt;
		proc = pproc;
		pos = ppos;
	}

	evento(){}

	bool operator< (const evento o) const {
		int ordem[] = {-1, 2, 5, 3, 4, 1, 6};
		return (t < o.t || (t == o.t && ordem[tipo] < ordem[o.tipo]));
	}
};

extern multiset<evento> eventos;

evento prox_evento();

void adiciona_evento(int, int, int, int);

#endif
