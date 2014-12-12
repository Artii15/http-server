all: server
server: obj/main.o obj/server.o
	g++ obj/main.o obj/server.o -Wall -o server
obj/main.o: src/main.cpp
	g++ src/main.cpp -o $@ -Wall -c -I inc
obj/server.o: src/server.cpp inc/server.h
	g++ src/server.cpp -o $@ -Wall -c -I inc
clean:
	rm -f obj/*
