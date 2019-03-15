EXE = concurrent_hash_map

SRC = $(wildcard *.cpp)
HDR = $(wildcard *.h)
OBJ = $(SRC:.cpp=.o)
DR = $(SRC:.cpp=.d)

CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++17

.PHONY: all
all: $(EXE)


$(EXE): $(OBJ)
	$(CXX)   $^ -lrt -ltbb  -o $@ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run: clean all
	./$(EXE)

.PHONY: clean
clean: 
	rm -f $(EXE) $(OBJ) $(DR)