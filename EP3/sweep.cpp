/*** Equipe ********************************************
Nome: Thiago Estrela Montenegro 	    NUSP: 9762873
Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370
*******************************************************/

#include <bits/stdc++.h>
using namespace std;

#include "sweep.h"

multiset<evento> eventos;

evento prox_evento(){
	evento x = *eventos.begin();
	eventos.erase(eventos.begin());
	return x;
}

void adiciona_evento(int t, int tipo, int proc, int pos){
	eventos.insert(evento(tipo, t, proc, pos));
}
