##################################################
### Makefile para gerar os binarios do arcabouco.
### 
### Autor : RLB
################################################## 

CC=gcc
CXX=g++
MYCFLAGS= -I. -ansi -Wall -g -O0 $(CFLAGS)
MYLFLAGS= -lm $(LFLAGS)

ALLDEPS = \
CESPDIN.H\
EXBESTAT.H\
GERAALT.H\
INTRPCNT.H\
TABSIMB.H\
TST_ESPC.H\
CONTA.H\
GENERICO.H\
INTRPCED.H\
LERPARM.H\
TESTALT.H

CRIALIBOBJ = \
CESPDIN.o\
GERAALT.o\
INTRPCNT.o\
PRINCIP.o\
CONTA.o\
GENERICO.o\
INTRPCED.o\
LERPARM.o\
TABSIMB.o

ALEATORIOOBJ = \
$(CRIALIBOBJ)\
TESTALT.o

CONTADORESOBJ = \
$(CRIALIBOBJ)\
TESTCNT.o

CEDOBJ = \
$(CRIALIBOBJ)\
TESTCED.o

GENOBJ = \
$(CRIALIBOBJ)\
TESTGEN.o

LEROBJ = \
$(CRIALIBOBJ)\
TESTLER.o
	
TBSOBJ = \
$(CRIALIBOBJ)\
TESTTBS.o

PERMUTAOBJ = \
GERAALT.o\
GERAPERMUTACAO.o

ALLTARGS = \
libArcaboucoTeste.a\
TesteAleatorio\
TesteContadores\
TesteControleEspaco\
TesteFuncoes\
TesteLerParametros\
TesteTabelaSimbolos\
GeraPermutacao


libArcaboucoTeste.a: $(CRIALIBOBJ)
	ar rcs $@ $^

all:$(ALLTARGS)

%.o: %.c $(ALLDEPS)
	$(CC) -c -o $@ $(MYCFLAGS) $<

TesteAleatorio: $(ALEATORIOOBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)

TesteContadores: $(CONTADORESOBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)

TesteControleEspaco: $(CEDOBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)
	
TesteFuncoes: $(GENOBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)
	
TesteLerParametros: $(LEROBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)
	
TesteTabelaSimbolos: $(TBSOBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)
	
GeraPermutacao: $(PERMUTAOBJ)
	$(CC) -o $@ $^  $(MYCFLAGS) $(MYLFLAGS)
	


.PHONY: clean
clean:
	rm -f ./*.o *~ core ./*~  ./lib*
	rm $(ALLTARGS) 2>/dev/null || true

