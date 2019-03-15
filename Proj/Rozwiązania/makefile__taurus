EXE = zadanie_x

SRC = $(wildcard *.cpp)
HDR = $(wildcard *.h)
OBJ = $(SRC:.cpp=.o)
DR = $(SRC:.cpp=.d)

CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++14

TBB_INSTALL_DIR = /home/stud2016/6mizera2/tbb-2019_U2
TBB_INCLUDE = $(TBB_INSTALL_DIR)/include
TBB_LIBRARY_RELEASE = $(TBB_INSTALL_DIR)/build/linux_intel64_gcc_cc6.3.0_libc2.24_kernel4.9.0_release
TBB_LIBRARY_DEBUG = $(TBB_INSTALL_DIR)/build/linux_intel64_gcc_cc6.3.0_libc2.24_kernel4.9.0_debug

.PHONY: all
all: $(EXE)

$(EXE): $(OBJ)
	$(CXX)   $^ -Wl,-rpath,$(TBB_LIBRARY_RELEASE) -L$(TBB_LIBRARY_RELEASE) -lrt  -ltbb  -o $@ 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(TBB_INCLUDE) -c $< -o $@

.PHONY: run
run: clean all
	@./$(EXE)

.PHONY: valgrind
valgrind: $(EXE )
	@valgrind  ./$(EXE )

.PHONY: clean
clean: 
	@rm -f $(EXE ) $(OBJ) $(DR)


