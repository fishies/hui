CXX = g++
CXXFLAGS = -std=c++17 -Wall -fext-numeric-literals
LDLIBS = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

hui: main.o
	$(CXX) $(CXXFLAGS) -o $@ $(LDLIBS) main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c main.cpp $(LDLIBS)
