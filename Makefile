all: server.o tcp_server.o
	g++ -std=c++11 server.o tcp_server.o -o server /usr/lib64/libboost_system.so.1.53.0 /usr/lib64/libboost_thread-mt.so.1.53.0

server.o: server.cpp
	g++ -std=c++11 -c server.cpp

tcp_server.o: tcp_server.cpp tcp_server.h
	g++ -std=c++11 -c tcp_server.cpp

clean:
	rm *.o
	rm server

