COMPILER=gcc
MON_OUTPUT_FILE=mon/bikeSharingMON.out
NYC_OUTPUT_FILE=nyc/bikeSharingNYC.out
CFLAGS=-Wall -pedantic -std=c99
DFLAGS=$(CFLAGS) -g -fsanitize=address
TRASHFILES=*.out *.o lib/*.o lib/*.out mon/*.out mon/*.o nyc/*.out nyc/*.o adt/*.o adt/*.out

mon/bikeSharingMON.out: mon/bikeSharingMON.c lib/frontLib.o adt/stationsADT.o lib/htmlTable.o
	@$(COMPILER) $(CFLAGS) mon/bikeSharingMON.c lib/frontLib.o adt/stationsADT.o lib/htmlTable.o -o $(MON_OUTPUT_FILE)

nyc/bikeSharingNYC.out: nyc/bikeSharingNYC.c lib/frontLib.o adt/stationsADT.o lib/htmlTable.o
	@$(COMPILER) $(CFLAGS) nyc/bikeSharingNYC.c lib/frontLib.o adt/stationsADT.o lib/htmlTable.o -o $(NYC_OUTPUT_FILE)

lib/frontLib.o: lib/frontLib.c
	@$(COMPILER) -c lib/frontLib.c $(CFLAGS) -o lib/frontLib.o

lib/htmlTable.o: lib/htmlTable.c
	@$(COMPILER) -c lib/htmlTable.c $(CFLAGS) -o lib/htmlTable.o

adt/stationsADT.o: adt/stationsADT.c
	@$(COMPILER) -c adt/stationsADT.c $(CFLAGS) -o adt/stationsADT.o

clean:
	@rm -f $(TRASHFILES)
	