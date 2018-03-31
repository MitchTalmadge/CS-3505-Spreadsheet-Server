server: main.h main.cpp dependency_graph.o
	g++ -std=c++11 dependency_graph.o main.cpp -o server

dependency_graph.o: dependency_graph.h dependency_graph.cpp
	g++ -std=c++11 -c dependency_graph.cpp -o dependency_graph.o