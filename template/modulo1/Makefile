CC=gcc
CFLAGS=-I. -I../headers -L../lib -O0 -fpermissive -g
+CFLAGS=-Wall

all: teste-modulo1

modulo1: 
	$(CC) $(CFLAGS) -c -x c++ MODULO1.c -o ../obj/MODULO1.o
teste-modulo1: modulo1
	$(CC) $(CFLAGS) -c -x c++ TESTEMODULO1.c -o ../obj/TESTEMODULO1.o
	$(CC) $(CFLAGS) -o ../bin/teste_modulo1.exe -lArcaboucoTeste ../obj/MODULO1.o ../obj/TESTEMODULO1.o
clean:
	rm *.o
	rm *.exe
