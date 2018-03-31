server: dependency_graph.o
	g++ -std=c++11 dependency_graph.o -o server

dependency_graph.o: dependency_graph.h dependency_graph.cpp
	g++ -c dependency_graph.cpp -o dependency_graph.o