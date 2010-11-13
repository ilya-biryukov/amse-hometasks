CC = g++
CFLAGS = -c -Wall

all: arithmetics

arithmetics: main.o utils.o largeint.o
	$(CC) main.o utils.o largeint.o -o arithmetics

main.o: main.cpp utils.h largeint.h
	$(CC) $(CFLAGS) main.cpp

utils.o: utils.cpp
	$(CC) $(CFLAGS) utils.cpp

largeint.o: largeint.cpp
	$(CC) $(CFLAGS) largeint.cpp

clean:
	rm -rf *.o arithmetics
