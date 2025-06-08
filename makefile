all:
	g++ -o main main.cpp $(wildcard src/classes/*.cpp)