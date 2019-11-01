CC=g++
CCFLAGS=-Wall -std=c++11 \
			$(shell perl -MExtUtils::Embed -e ccopts)
LD=g++
LDFLAGS=-Wall -std=c++11 \
			$(shell perl -MExtUtils::Embed -e ldopts)

all: interpreter.out

PROGRAMOBJS = interpreter.o parser.o expr.o data.o

interpreter.o: interpreter.cpp
	$(CC) -c $< $(CCFLAGS) -Wwrite-strings

parser.o: parser.cpp
	$(CC) -c $< $(CCFLAGS)

expr.o: expr.cpp
	$(CC) -c $< $(CCFLAGS)

data.o: data.cpp
	$(CC) -c $< $(CCFLAGS)

#LDFLAGS tem que ser depois por algum motivo
interpreter.out: $(PROGRAMOBJS)
	$(LD) -o $@ $(PROGRAMOBJS) $(LDFLAGS)

clean:
	rm *.o *.out
