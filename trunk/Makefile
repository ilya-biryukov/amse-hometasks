CC = g++
CFLAGS = -c -Wall

all: arithmetics

arithmetics: main.o utils.o
	$(CC) main.o utils.o -o arithmetics

main.o: main.cpp utils.h
	$(CC) $(CFLAGS) main.cpp

utils.o: utils.cpp
	$(CC) $(CFLAGS) utils.cpp

clean:
	rm -rf *.o arithmetics
