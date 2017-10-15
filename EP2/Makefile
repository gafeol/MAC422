CC=gcc -std=gnu99

all: ep2 

ep2: ep2.o queue.o heap.o
		$(CC) -pthread -lmemusage -g $^ -o $@

ep2.o: ep2.c global.h heap.h
		$(CC) ep2.c -pthread -g -c -o $@

#ciclista.o: ciclista.c ciclista.h global.h
#		$(CC) ciclista.c -pthread -c -o $@

#pista.o: pista.c pista.h global.h
#		$(CC) pista.c -pthread -c -o $@

queue.o: queue.c queue.h
		$(CC) queue.c -g -c -o $@

heap.o: heap.c heap.h
		$(CC) heap.c -g -c -o $@

clean: 
	rm ep2 *.o *~

