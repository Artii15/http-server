all: server
server: obj/main.o obj/Server.o
	g++ obj/main.o obj/Server.o -Wall -o server
obj/main.o: src/main.cpp inc/Server.h
	g++ src/main.cpp -o $@ -Wall -c -I inc
obj/Server.o: src/Server.cpp inc/Server.h
	g++ src/Server.cpp -o $@ -Wall -c -I inc
clean:
	rm -f obj/*
