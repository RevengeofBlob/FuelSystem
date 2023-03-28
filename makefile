CXX = g++
CXXFLAGS = -Wall -g

mytest: fuel.o mytest.cpp
	$(CXX) $(CXXFLAGS) fuel.o mytest.cpp -o mytest

fuel.o: fuel.h fuel.cpp
	$(CXX) $(CXXFLAGS) -c fuel.cpp
