CFLAGS=-W -Wall -g -I/usr/local/include
LDFLAGS=-L/usr/local/lib
PROGRAMS=sorttest cskmtest wordfreq
OBJECTS=sort.o
LIBRARIES=-lADTs

all: $(PROGRAMS)

sorttest: sorttest.o sort.o
	gcc $(LDFLAGS) -o sorttest $^ $(LIBRARIES)

cskmtest: cskmtest.o sort.o
	gcc $(LDFLAGS) -o cskmtest $^ $(LIBRARIES)

wordfreq: wordfreq.o sort.o
	gcc $(LDFLAGS) -o wordfreq $^ $(LIBRARIES)

sorttest.o: sorttest.c sort.h
csdmtest.o: cskmtest.c sort.h
wordfreq.o: wordfreq.c sort.h

clean:
	rm -f $(PROGRAMS) $(OBJECTS) sorttest.o cskmtest.o wordfreq.o
