server: dependency_graph.o
	g++ -std=c++11 dependency_graph.o -o server

dependency_graph.o: dependency_graph.h dependency_graph.cpp
	g++ dependency_graph.h dependency_graph.cpp -c -o dependency_graph.o