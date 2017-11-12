# Equipe
# Nome: Thiago Estrela Montenegro 	NUSP: 9762873
# Nome: Gabriel Fernandes de Oliveira     NUSP: 9345370

CC=gcc -std=gnu99

all: ep2 

ep2: ep2.o queue.o heap.o
		$(CC) -pthread -g $^ -o $@

ep2.o: ep2.c global.h heap.h
		$(CC) ep2.c -pthread -g -c -o $@

queue.o: queue.c queue.h
		$(CC) queue.c -g -c -o $@

heap.o: heap.c heap.h
		$(CC) heap.c -g -c -o $@

clean: 
	rm ep2 *.o *~

