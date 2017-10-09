CC=gcc -std=c99

all: ep2 

ep2: ep2.o ciclista.o pista.o queue.o
		$(CC) -pthread $^ -o $@

ep2.o: ep2.c ciclista.h pista.h queue.h aleatorio.h global.h
		$(CC) ep2.c -c -o $@

ciclista.o: ciclista.c ciclista.h pista.h aleatorio.h global.h
		$(CC) ciclista.c -pthread -c -o $@

pista.o: pista.c ciclista.h pista.h ciclista.h aleatorio.h global.h
		$(CC) pista.c -pthread -c -o $@

queue.o: queue.c
		$(CC) queue.c -c -o $@

clean: 
	rm ep2 *.o *~

