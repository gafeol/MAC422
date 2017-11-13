# Equipe
# Nome: Thiago Estrela Montenegro 	NUSP: 9762873
# Nome: Gabriel Fernandes de Oliveira   NUSP: 9345370

CC=g++

all: ep3 

ep3: ep3.o processo.o sweep.o
		$(CC) -g $^ -o $@

ep3.o: ep3.cpp processo.h sweep.h
		$(CC) ep3.cpp -g -c -o $@

processo.o: processo.cpp processo.h
		$(CC) processo.cpp -g -c -o $@

sweep.o: sweep.cpp sweep.h
		$(CC) sweep.cpp -g -c -o $@

clean: 
	rm ep3 *.o *~

