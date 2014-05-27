#Compilador usado
CC = gcc

#Definiendo las opciones de compilacion
CFLAGS= -Wall -O0 -g

default: clean compile

help:
	@echo "Usar:"
	@echo "   compile:     Compilar"
	@echo "   clean:   Eliminar ejecutables"
	@echo "   tar:     Empaquetar"

compile: cola_circular.c buffer.c
	$(CC) $(CFLAGS) -c cola_circular.c
	$(CC) $(CFLAGS) -c buffer.c
	$(CC) $(CFLAGS) -c semaforo.c

	$(CC) $(CFLAGS) -c inicializa.c
	$(CC) $(CFLAGS) -o inicializa inicializa.o buffer.o cola_circular.o semaforo.o
	
	$(CC) $(CFLAGS) -c finaliza.c
	$(CC) $(CFLAGS) -o finaliza finaliza.o buffer.o cola_circular.o semaforo.o

	$(CC) $(CFLAGS) -c productores.c
	$(CC) $(CFLAGS) -o productores productores.o buffer.o cola_circular.o semaforo.o
	$(CC) $(CFLAGS) -c consumidores.c
	$(CC) $(CFLAGS) -o consumidores consumidores.o buffer.o cola_circular.o semaforo.o


clean:
	rm -f  *.o *.c.bak buffer inicializa productores consumidores finaliza

tar: buffer.c sem.c inicializa.c productores.c consumidores.c finaliza.c
	tar czvf p2.tar.gz *.c *.h Makefile README
	
