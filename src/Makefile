CC=g++
CCFLAGS=-Wall -std=c++14 -g
EMBEDCCFLAGS= $(shell perl -MExtUtils::Embed -e ccopts)
LD=g++
LDFLAGS=-Wall -std=c++14 -g \
			$(shell perl -MExtUtils::Embed -e ldopts)



all: interpreter

AUX_OBJS = parser.o expr.o data.o evaluator.o stmt.o error.o environment.o

aux: $(AUX_OBJS)

PROGRAMOBJS = $(AUX_OBJS) main.o

main.o: main.cpp
	$(CC) -c $< $(CCFLAGS) $(EMBEDCCFLAGS)

parser.o: parser.cpp
	$(CC) -c $< $(CCFLAGS)

expr.o: expr.cpp
	$(CC) -c $< $(CCFLAGS)

data.o: data.cpp
	$(CC) -c $< $(CCFLAGS)

evaluator.o: evaluator.cpp
	$(CC) -c $< $(CCFLAGS)

error.o: error.cpp
	$(CC) -c $< $(CCFLAGS)

stmt.o: stmt.cpp
	$(CC) -c $< $(CCFLAGS)

environment.o: environment.cpp
	$(CC) -c $< $(CCFLAGS)

interpreter: $(PROGRAMOBJS)
	$(LD) -o $@ $(PROGRAMOBJS) $(LDFLAGS)

clean:
	rm -f *.o interpreter
