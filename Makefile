CXX = g++
CXXFLAGS = -std=c++17 -O3 -Iinclude

SRC = src/main.cpp src/Graph.cpp
OUT = td

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)