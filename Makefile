CC = g++
CFLAGS = -c

all: expressions

expressions: main.o function.o utils.o
	$(CC) main.o function.o utils.o -o expressions

main.o: function.h utils.h main.cpp
	$(CC) $(CFLAGS) main.cpp

function.o: function.cpp
	$(CC) $(CFLAGS) function.cpp

utils.o: function.h utils.cpp 
	$(CC) $(CFLAGS) utils.cpp

run: expressions
	./expressions
clean: 
	rm -f *.o expressions 
