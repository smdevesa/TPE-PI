COMPILER=gcc
OUTPUT_FILE=prueba.out
CFLAGS=-Wall -pedantic -std=c99
DEBUGFLAGS=$(CFLAGS) -g -fsanitize=address

mon/bikeSharingMON.out: mon/bikeSharingMON.c lib/frontLib.o
	$(COMPILER) -o $(OUTPUT_FILE) $(DEBUGFLAGS) mon/bikeSharingMON.c lib/frontLib.o

lib/frontLib.o: lib/frontLib.c
	$(COMPILER) -c lib/frontLib.c $(DEBUGFLAGS) -o lib/frontLib.o

clean:
	rm -f *.out *.o lib/*.o mon/*.out
