all: rshell

rshell:
	mkdir -p ./bin
	g++ -Wall -Werror -ansi  -pedantic ./src/rshell.cpp -o ./bin/rshell
