CC = g++
CFLAGS = -c

all: unallowed-call

unallowed-call: main.o
	$(CC) main.o -o unallowed-call

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm *.o unallowed-call
