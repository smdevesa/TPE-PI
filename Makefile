COMPILER=gcc
MON_OUTPUT_FILE=mon/bikeSharingMON.out
CFLAGS=-Wall -pedantic -std=c99
DEBUGFLAGS=$(CFLAGS) -g -fsanitize=address
TRASHFILES=*.out *.o lib/*.o lib/*.out mon/*.out mon/*.o nyc/*.out nyc/*.o adt/*.o adt/*.out

mon/bikeSharingMON.out: mon/bikeSharingMON.c lib/frontLib.o adt/stationsADT.o
	@$(COMPILER) $(DEBUGFLAGS) mon/bikeSharingMON.c lib/frontLib.o adt/stationsADT.o -o $(MON_OUTPUT_FILE)

lib/frontLib.o: lib/frontLib.c
	@$(COMPILER) -c lib/frontLib.c $(DEBUGFLAGS) -o lib/frontLib.o

adt/stationsADT.o: adt/stationsADT.c
	@$(COMPILER) -c adt/stationsADT.c $(DEBUGFLAGS) -o adt/stationsADT.o

clean:
	@rm -f $(TRASHFILES)