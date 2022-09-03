.default: all

all: Task3

clean:
	rm -rf Task3 *.o *.dSYM

Task3: Task1.o Task3.o Sorts.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 