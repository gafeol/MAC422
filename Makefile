CC=gcc

all: ep1 ep1sh

ep1sh: ep1sh.c
		$(CC) ep1sh.c -lreadline -o $@

ep1: ep1.o sjf.o rr.o p.o print.o process.o queue.o heap.o calctime.o
		$(CC) -pthread $^ -o $@

ep1.o: ep1.c print.h p.h rr.h sjf.h
		$(CC) ep1.c -c -o $@

sjf.o: sjf.c sjf.h process.h queue.h heap.h print.h calctime.h constants.h
		$(CC) sjf.c -c -o $@

rr.o: rr.c rr.h process.h queue.h heap.h print.h calctime.h constants.h
		$(CC) rr.c -c -o $@

p.o: p.c p.h process.h queue.h heap.h print.h calctime.h constants.h
		$(CC) p.c -c -o $@

process.o: process.c process.h
		$(CC) process.c -c -o $@

heap.o: heap.c heap.h
		$(CC) heap.c -c -o $@

queue.o: queue.c queue.h
		$(CC) queue.c -c -o $@

calctime.o: calctime.c calctime.h
		$(CC) calctime.c -c -o $@

constants.h:

clean: 
	rm ep1 ep1sh *.o *~

