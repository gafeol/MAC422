#include <bits/stdc++.h>
using namespace std;

#include "fifo.h"
#include "global.h"
#include "processo.h"
#include "memory.h"

#define debug(args...) fprintf(stderr,args)

void fifo(int pos_virt){
	while(!fila_fis.empty() && livre[fila_fis.front()] > 0){
		livre[fila_fis.front()]--;
		fila_fis.pop();
	}
	assert(!fila_fis.empty());
	int pag = fila_fis.front();
	fila_fis.pop();
	fila_fis.push(pag);

	substitui_pag(pag, pos_virt);
}
