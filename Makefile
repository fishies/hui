CXX = g++
CXXFLAGS = -std=c++17 -Wall -fext-numeric-literals
LDLIBS = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

hui: main.o
	$(CXX) $(CXXFLAGS) main.o systems.o ecs.o -o $@ $(LDLIBS)

main.o: ecs.o systems.o components.o main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c main.cpp $(LDLIBS)

systems.o: systems.hpp components.hpp components.o
	$(CXX) $(CXXFLAGS) -o $@ -c systems.cpp $(LDLIBS)

components.o: components.hpp ecs.hpp ecs.o
	$(CXX) $(CXXFLAGS) -o $@ -c components.hpp $(LDLIBS)

ecs.o: ecs.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c ecs.cpp $(LDLIBS)