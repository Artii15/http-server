all: server
server: obj/main.o
	g++ obj/main.o -Wall -o server
obj/main.o: src/main.cpp
	g++ src/main.cpp -o $@ -Wall -c -I inc
clean:
	rm -f obj/*
