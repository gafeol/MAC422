#ifndef PROCESSO_H
#define PROCESSO_H

#include <bits/stdc++.h>
using namespace std;

struct processo{
	int t0, tf, b;
	string nome;
	map<int, int> memf; // posicao na memoria fisica
	int pos_virt; //posicao inicial na memoria virtual

	processo(int tt0, int ttf, int bb, string nn){
		t0 = tt0;
		tf = ttf;
		bb = b;
		nome = nn;
		memf.clear();
		pos_virt = -1;
	}

	processo(){}
};

processo cria_processo(int t0, int tf, int b, string nome);

#endif
