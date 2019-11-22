CC=g++
CCFLAGS=-Wall -std=c++14 -g
EMBEDCCFLAGS= $(shell perl -MExtUtils::Embed -e ccopts)
LD=g++
LDFLAGS=-Wall -std=c++14 -g \
			$(shell perl -MExtUtils::Embed -e ldopts)

all: interpreter

AUX_OBJS = parser.o \
		   expr.o \
		   data.o \
		   evaluator.o \
		   stmt.o \
		   error.o \
		   environment.o

aux: $(AUX_OBJS)

PROGRAMOBJS = $(AUX_OBJS) main.o

main.o: src/main.cpp
	$(CC) -c $< $(CCFLAGS) $(EMBEDCCFLAGS)

parser.o: src/parser.cpp
	$(CC) -c $< $(CCFLAGS)

expr.o: src/expr.cpp
	$(CC) -c $< $(CCFLAGS)

data.o: src/data.cpp
	$(CC) -c $< $(CCFLAGS)

evaluator.o: src/evaluator.cpp
	$(CC) -c $< $(CCFLAGS)

error.o: src/error.cpp
	$(CC) -c $< $(CCFLAGS)

stmt.o: src/stmt.cpp
	$(CC) -c $< $(CCFLAGS)

environment.o: src/environment.cpp
	$(CC) -c $< $(CCFLAGS)

interpreter: $(PROGRAMOBJS)
	$(LD) -o $@ $(PROGRAMOBJS) $(LDFLAGS)
	mv *.o obj
	rm -f *.o

clean:
	rm -f *.o interpreter
