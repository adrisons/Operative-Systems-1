#Compilador usado
CC = gcc

#Definiendo las opciones de compilacion
CFLAGS= -Wall -O0 -g

default:
	@echo "Usar:"
	@echo "   all:     Compilar"
	@echo "   clean:   Eliminar ejecutables"
	@echo "   tar:     Empaquetar"

all: copy.c
	$(CC) $(CFLAGS) -o copy copy.c

clean:
	rm copy

tar: copy.c
	tar czvf p1.tar.gz copy.c Makefile README
