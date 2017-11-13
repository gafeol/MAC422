#ifndef SWEEP_H
#define SWEEP_H

#include <bits/stdc++.h>
using namespace std;

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
		return (t < o.t);
	}
};

extern multiset<evento> eventos;

evento prox_evento();

void adiciona_evento(int, int, int, int);

#endif
