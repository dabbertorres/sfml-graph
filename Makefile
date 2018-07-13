CXXC     ?= g++
CXXFLAGS += -std=c++17 -Wall -Wextra
LDFLAGS  += -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: clean

test: test.o Graph.o
	$(CXXC) -o $@ $(LDFLAGS) $^

test.o: test.cpp Graph.hpp
	$(CXXC) $(CXXFLAGS) -c -o $@ $<

Graph.o: Graph.cpp Graph.hpp
	$(CXXC) $(CXXFLAGS) -c -o $@ $<

clean:
	@rm -f test test.o Graph.o

