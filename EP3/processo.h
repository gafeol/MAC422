#ifndef PROCESSO_H
#define PROCESSO_H

#include <bits/stdc++.h>
using namespace std;

struct processo{
	int t0, tf, b;

	int pid;
	string nome;
	map<int, int> memf; // posicao na memoria fisica
	int pos_virt; //posicao inicial na memoria virtual

	processo(int tt0, int ttf, int bb, string nn){
		t0 = tt0;
		tf = ttf;
		b = bb;
		nome = nn;
		memf.clear();
		pos_virt = -1;
	}

	processo(){}
};

extern vector<processo> processos;

processo cria_processo(int t0, int tf, int b, string nome);

void aloca_processo(int pro, int alg_aloc);

void remove_processo(int pro, int alg_subs, int alg_aloc);

void acessa_pag(int pro, int pos, int alg_subs);

#endif
