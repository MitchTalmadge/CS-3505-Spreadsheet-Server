server:
	g++ -std=c++11 dependency_graph.o -o server

dependency_graph.o: dependency_graph.h dependency_graph.cpp
	g++ dependency_graph.h dependency_graph.cpp -c dependency_graph.o