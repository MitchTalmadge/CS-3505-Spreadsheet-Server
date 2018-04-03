server: src/main.h src/main.cpp lib/dependency_graph.o
	g++ -std=c++11 lib/dependency_graph.o src/main.cpp -o bin/server

lib/dependency_graph.o: src/util/dependency_graph.h src/util/dependency_graph.cpp
	g++ -std=c++11 -c src/util/dependency_graph.cpp -o lib/dependency_graph.o