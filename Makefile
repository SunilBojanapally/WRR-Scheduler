CC=gcc
CFLAGS=-Wall

wrr: wrr.c
	$(CC) -o wrr wrr.c 
    
clean:
	rm -f wrr wrr.o
