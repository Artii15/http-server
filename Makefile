all: server
server: obj/ConnectionHandler.o obj/DateTime.o obj/HttpConnectionHandler.o obj/HttpException.o obj/HttpHeaderReader.o obj/main.o obj/Server.o
	g++ obj/ConnectionHandler.o obj/DateTime.o obj/HttpConnectionHandler.o obj/HttpException.o obj/HttpHeaderReader.o obj/main.o obj/Server.o -Wall -O3 -lpthread -lboost_regex -o server
obj/ConnectionHandler.o: src/ConnectionHandler.cpp inc/ConnectionHandler.h
	g++ src/ConnectionHandler.cpp -o $@ -Wall -O3 -c -I inc
obj/DateTime.o: src/DateTime.cpp inc/DateTime.h
	g++ src/DateTime.cpp -o $@ -Wall -O3 -c -I inc
obj/HttpConnectionHandler.o: src/HttpConnectionHandler.cpp inc/HttpConnectionHandler.h inc/ConnectionHandler.h inc/HttpHeaderReader.h inc/HttpException.h
	g++ src/HttpConnectionHandler.cpp -o $@ -Wall -O3 -c -I inc
obj/HttpException.o: src/HttpException.cpp inc/HttpException.h
	g++ src/HttpException.cpp -o $@ -Wall -O3 -c -I inc
obj/HttpHeaderReader.o: src/HttpHeaderReader.cpp inc/HttpHeaderReader.h
	g++ src/HttpHeaderReader.cpp -o $@ -Wall -O3 -c -I inc
obj/main.o: src/main.cpp inc/Server.h inc/HttpConnectionHandler.h inc/ConnectionHandler.h inc/HttpHeaderReader.h
	g++ src/main.cpp -o $@ -Wall -O3 -c -I inc
obj/Server.o: src/Server.cpp inc/Server.h inc/HttpConnectionHandler.h inc/ConnectionHandler.h inc/HttpHeaderReader.h
	g++ src/Server.cpp -o $@ -Wall -O3 -c -I inc
clean:
	rm -f obj/*
