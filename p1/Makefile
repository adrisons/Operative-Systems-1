#Compilador usado
CC = gcc

#Definiendo las opciones de compilacion
CFLAGS= -Wall -O0 -g

default: help

help:
	@echo " +----------- Makefile ----------+"
	@echo " | compile: Compilar             |"
	@echo " | clean:   Eliminar ejecutables |"
	@echo " | tar:     Empaquetar           |"
	@echo " | help:    Muestra esta ayuda   |"
	@echo " +-------------------------------+"


compile: copy.c
	$(CC) $(CFLAGS) -o copy copy.c

clean:
	rm copy

tar: copy.c
	tar czvf p1.tar.gz *.c *.h Makefile
