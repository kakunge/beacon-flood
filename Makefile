LDLIBS=-lpcap

all: beacon-flood

main.o: main.cpp

radiotap.o: radiotap.h radiotap.cpp

beacon-flood: main.o radiotap.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f beacon-flood *.o