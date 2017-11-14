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
		int ordem[] = {-1, 2, 5, 3, 4, 1};
		return (t < o.t || (t == o.t && ordem[tipo] < ordem[o.tipo]));
	}
};

extern multiset<evento> eventos;

evento prox_evento();

void adiciona_evento(int, int, int, int);

#endif
