.default: all

all: Task5

clean:
	rm -rf program *.o *.dSYM

Task5: stream.o Task5.o sorts.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 