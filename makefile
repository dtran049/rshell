all: rshell

rshell:
	mkdir -p ./bin
	g++ -Wall -Werror -ansi  -pedantic ./src/rshell.cpp -o ./bin/rshell

rm:
	mkdir -p ./bin
	g++ -Wall -Werror --std=c++11  -pedantic ./src/rm.cpp -o ./bin/rm

mv:
	mkdir -p ./bin
	g++ -Wall -Werror --std=c++11  -pedantic ./src/mv.cpp -o ./bin/mv
