.default: all

all: Task4

clean:
	rm -rf Task4 *.o *.dSYM

Task4: Task1.o Task4.o Sorts.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 