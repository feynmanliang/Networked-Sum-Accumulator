CC=gcc
CFLAGS= -Wall -pedantic -g

CFILES=server.c client.c
HFILES=
OFILES=server.o client.o

all: server client

%.o:	%.c $(HFILES)
	$(CC) -c $(CFLAGS) $< -o $@

server: server.o $(HFILES)
	 $(CC) $(CFLAGS) server.o -o server

client: client.o $(HFILES)
	 $(CC) $(CFLAGS) client.o -o client

clean:	
	rm -f *~ client server.o client.o

