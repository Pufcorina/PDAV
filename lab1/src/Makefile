CC=gcc

CFLAGS=-c -Wall

all: final

final: main.o rw.o
	$(CC) main.o rw.o -o final

main.o: main.c
	$(CC) $(CFLAGS) main.c

rw.o: rw.c
		$(CC) $(CFLAGS) rw.c

clean:
	rm -rf *o final
