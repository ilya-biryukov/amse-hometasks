CC = g++
CFLAGS = -c -Wall

all: arithmetics

arithmetics: main.o utils.o largeint.o sharedptr.o
	$(CC) main.o utils.o largeint.o sharedptr.o -o arithmetics

main.o: main.cpp utils.h largeint.h
	$(CC) $(CFLAGS) main.cpp

utils.o: utils.cpp largeint.h
	$(CC) $(CFLAGS) utils.cpp

largeint.o: largeint.cpp largeint.h
	$(CC) $(CFLAGS) largeint.cpp

sharedptr.o: sharedptr.cpp sharedptr.h
	$(CC) $(CFLAGS) sharedptr.cpp
clean:
	rm -rf *.o arithmetics
