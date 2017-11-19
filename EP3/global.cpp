/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;

#include "global.h"

int ualoc, tam_pag, dt;
int total, virt;

map<int, map<int, int> > qtd_aces;

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

map<int, int> freq;
int val[2];
set<int> pos[2];
set<pair<int, int> > proc;

double tempo_busca;
int page_fault;

int asserting;
