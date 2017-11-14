#ifndef MEMORY_H
#define MEMORY_H

#include <bits/stdc++.h>
using namespace std;

void seta_virtual(int i, int tam, int p);
void remove_virtual(int p, int alg_subs);
void procura_fis(int, int);
void aloca_fis(int, int);
void libera_fis(int);
void modifica_fis(int, int);
void substitui_pag(int, int);
void compacta();

#endif
