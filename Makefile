.default: all

all: Task1 Task2 Task3 Task4 Task5 Task5Server

clean:
	rm -rf Task3 *.o *.dSYM
	rm -rf Task1 *.o *.dSYM
	rm -rf Task2 *.o *.dSYM
	rm -rf Task4 *.o *.dSYM
	rm -rf Task5 *.o *.dSYM
	rm -rf Task5Server *.o *.dSYM

Task5Server: Stream.o Task5Server.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

Task1: Task1.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

Task2: Task1Filter.o Task2.o Utils.o OutputHandler.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

Task3: Task1Filter.o Task3.o Utils.o OutputHandler.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

Task4: Task1Filter.o Task4.o Utils.o OutputHandler.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

Task5: Task5.o Utils.o
	g++ -Wall -Werror -std=c++14 -g -O -pthread -o $@ $^

%.o: %.cpp
	g++  -Wall -Werror -std=c++14 -g -O -c $^ 