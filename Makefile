CXX = g++
CXXFLAGS =
BIN = bin

.PHONY: all solution run_tests clean

all: solution run_tests

solution:
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) solution.cpp -o $(BIN)/solution

run_tests:
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) run_tests.cpp -o $(BIN)/run_tests

clean:
	rm -rf $(BIN)
