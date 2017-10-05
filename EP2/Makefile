CC=gcc

all: ep2

ep2: ep2.o
		$(CC) -pthread $^ -o $@

ep1.o: ep2.c ciclista.h
		$(CC) ep1.c -c -o $@

clean: 
	rm ep2 *.o *~

