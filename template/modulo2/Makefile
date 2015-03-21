CC=gcc
CFLAGS=-I. -I../headers -I../modulo1 -L../lib -O0 -fpermissive -g
+CFLAGS=-Wall

all: teste-modulo2
modulo1:
	cd ../modulo1/ && $(MAKE) modulo1
modulo2: 
	$(CC) $(CFLAGS) -c -x c++ MODULO2.c -o ../obj/MODULO2.o
teste-modulo2: modulo1 modulo2
	$(CC) $(CFLAGS) -c -x c++ TESTEMODULO2.c -o ../obj/TESTEMODULO2.o
	$(CC) $(CFLAGS) -o ../bin/teste_modulo2.exe -lArcaboucoTeste ../obj/MODULO1.o ../obj/MODULO2.o ../obj/TESTEMODULO2.o
clean:
	rm *.o
	rm *.exe
