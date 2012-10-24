CC=gcc
CFLAGS= -Wall -pedantic -g

CFILES=server.c client.c
HFILES=
OFILES=server.o client.o

all: server client

%.o:	%.c $(HFILES)
	$(CC) -c $(CFLAGS) $< -o $@

server: server.o $(HFILES)
	 $(CC) $(CFLAGS) server.o -o bin/server

client: client.o $(HFILES)
	 $(CC) $(CFLAGS) client.o -o bin/client

clean:	
	rm -f *~ bin/client bin/server server.o client.o

