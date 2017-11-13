#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
using namespace std;

const char EMPTY = 129;

extern int ualoc, tam_pag, dt;

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

int ceil(int, int);

extern queue<int> fila_fis;

#endif
