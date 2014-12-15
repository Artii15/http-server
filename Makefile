all: server
server: obj/ConnectionHandler.o obj/HttpConnectionHandler.o obj/main.o obj/Server.o
	g++ obj/ConnectionHandler.o obj/HttpConnectionHandler.o obj/main.o obj/Server.o -Wall -O3 -lpthread -o server
obj/ConnectionHandler.o: src/ConnectionHandler.cpp inc/ConnectionHandler.h
	g++ src/ConnectionHandler.cpp -o $@ -Wall -O3 -c -I inc
obj/HttpConnectionHandler.o: src/HttpConnectionHandler.cpp inc/HttpConnectionHandler.h inc/ConnectionHandler.h
	g++ src/HttpConnectionHandler.cpp -o $@ -Wall -O3 -c -I inc
obj/main.o: src/main.cpp inc/Server.h inc/HttpConnectionHandler.h inc/ConnectionHandler.h
	g++ src/main.cpp -o $@ -Wall -O3 -c -I inc
obj/Server.o: src/Server.cpp inc/Server.h inc/HttpConnectionHandler.h inc/ConnectionHandler.h
	g++ src/Server.cpp -o $@ -Wall -O3 -c -I inc
clean:
	rm -f obj/*
