#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
using namespace std;

const char EMPTY = 129;

const int OPT = 1;
const int FIFO = 2;
const int LRU2 = 3;
const int LRU4 = 4;

extern int ualoc, tam_pag, dt;
extern int total, virt;

extern map<int, map<int, int> > qtd_aces;

extern int nquad;

struct mem_virt{
	int ind;
	int pos_fis;
	mem_virt(int ii, int pp){
		ind = ii;
		pos_fis = pp;
	}
	mem_virt(){
		ind = EMPTY;
		pos_fis = EMPTY;
	}
};

struct mem_fis{
	int ind;
	int pos_virt;
	mem_fis(int ii, int pp){
		ind = ii;
		pos_virt = pp;
	}
	mem_fis(){
		ind = EMPTY;
		pos_virt = EMPTY;
	}
};

extern vector<mem_virt> MV;
extern vector<mem_fis> MF;
extern int *R;

extern int *livre;

extern bool** matriz_pag;

int ceil(int, int);

extern queue<int> fila_fis;
extern queue<int> pid_disp;

extern map<int, int> freq;
extern int val[2];

extern set<int> pos[2];
extern set<pair<int, int> > proc;

extern double tempo_busca;
extern int page_fault;

extern int asserting;

#endif
