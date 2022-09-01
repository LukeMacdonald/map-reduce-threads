.default: all

all: program

clean:
	rm -rf program *.o *.dSYM

program: main.o task1.o task2.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 