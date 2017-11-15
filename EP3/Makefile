# Equipe
# Nome: Thiago Estrela Montenegro 	NUSP: 9762873
# Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370

CC=g++

all: ep3 

ep3: ep3.o processo.o sweep.o memory.o global.o best_fit.o worst_fit.o fifo.o lru2.o lru4.o optimal.o quick_fit.o lista.o
		$(CC) -g $^ -o $@

ep3.o: ep3.cpp processo.h sweep.h memory.h global.h
		$(CC) ep3.cpp -g -c -o $@

processo.o: processo.cpp processo.h worst_fit.h best_fit.h quick_fit.h
		$(CC) processo.cpp -g -c -o $@

sweep.o: sweep.cpp sweep.h
		$(CC) sweep.cpp -g -c -o $@

memory.o: memory.cpp memory.h lru2.h
		$(CC) memory.cpp -g -c -o $@

global.o: global.cpp global.h
		$(CC) global.cpp -g -c -o $@

best_fit.o: best_fit.cpp best_fit.h
		$(CC) best_fit.cpp -g -c -o $@

worst_fit.o: worst_fit.cpp worst_fit.h
		$(CC) worst_fit.cpp -g -c -o $@

fifo.o: fifo.cpp fifo.h
		$(CC) fifo.cpp -g -c -o $@

lru2.o: lru2.cpp lru2.h
		$(CC) lru2.cpp -g -c -o $@

lru4.o: lru4.cpp lru4.h
		$(CC) lru4.cpp -g -c -o $@

optimal.o: optimal.cpp optimal.h
		$(CC) optimal.cpp -g -c -o $@

quick_fit.o: quick_fit.cpp quick_fit.h
		$(CC) quick_fit.cpp -g -c -o $@
		
lista.o: lista.cpp lista.h processo.h global.h
		$(CC) lista.cpp -g -c -o $@



clean: 
	rm ep3 *.o *~ -rf tmp/

