default: all


all: main internal_state
	g++ -std=c++14 -o plantergb main.o InternalState.o

main:
	g++ -std=c++14 -Wall -c main.cpp -o main.o

internal_state:
	g++ -std=c++14 -Wall -c InternalState.cpp -o InternalState.o

clean:
	rm -f *.o