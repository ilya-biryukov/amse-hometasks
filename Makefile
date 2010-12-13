CC = g++
CFLAGS = -c

all: expressions

expressions: main.o expression.o utils.o
	$(CC) main.o expression.o utils.o -o expressions

main.o: expression.h utils.h main.cpp
	$(CC) $(CFLAGS) main.cpp

expression.o: expression.cpp
	$(CC) $(CFLAGS) expression.cpp

utils.o: expression.h utils.cpp 
	$(CC) $(CFLAGS) utils.cpp

run: expressions
	./expressions
clean: 
	rm -f *.o expressions 
