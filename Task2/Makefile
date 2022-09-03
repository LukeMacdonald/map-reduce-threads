.default: all

all: Task2

clean:
	rm -rf Task2 *.o *.dSYM

Task2: Task1.o Task2.o Sorts.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 