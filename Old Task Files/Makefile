.default: all

all: program

clean:
	rm -rf program *.o *.dSYM

program: main.o task1.o task2.o task3.o sorts.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 