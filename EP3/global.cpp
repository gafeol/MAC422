#include <bits/stdc++.h>
using namespace std;

#include "global.h"

int ualoc, tam_pag, dt;
int total, virt;

int nquad;

vector<mem_virt> MV;
vector<mem_fis> MF;
int *R;

int *livre;

bool **matriz_pag;

int ceil(int a, int b){
	return a/b + (a%b != 0);
}

queue<int> fila_fis;
queue<int> pid_disp;
