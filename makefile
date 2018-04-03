# Entry point.
server: directories src/main.h src/main.cpp lib/dependency_graph.o
	g++ -std=c++11 lib/dependency_graph.o src/main.cpp -o bin/server

# Creates output directories. Lib is for object-file dependencies, bin is for final executable output.
directories:
	mkdir lib; mkdir bin

# Dependency Graph
lib/dependency_graph.o: src/util/dependency_graph.h src/util/dependency_graph.cpp
	g++ -std=c++11 -c src/util/dependency_graph.cpp -o lib/dependency_graph.o