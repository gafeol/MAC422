CC=gcc

all: ep2 

ep2: ep2.o
		$(CC) -pthread $^ -o $@

ep2.o: ep2.c ciclista.o pista.o queue.o
		$(CC) ep2.c -c -o $@

ciclista.o: ciclista.c ciclista.h pista.h aleatorio.h global.h
		$(CC) ciclista.c -c -o $@

pista.o: pista.c pista.h aleatorio.h global.h
		$(CC) pista.c -c -o $@

queue.o: queue.c
		$(CC) queue.c -c -o $@

clean: 
	rm ep2 *.o *~

