CXXC     ?= g++
CXXFLAGS += -std=c++17 -Wall -Wextra
LDFLAGS  += -lsfml-graphics -lsfml-window -lsfml-system -ldl

.PHONY: clean

test: test.o Space.o Function.o
	$(CXXC) -o $@ $^ $(LDFLAGS)

test.o: test.cpp Space.hpp
	$(CXXC) $(CXXFLAGS) -c -o $@ $<

Space.o: Space.cpp Space.hpp Function.hpp
	$(CXXC) $(CXXFLAGS) -c -o $@ $<

Function.o: Function.cpp Function.hpp
	$(CXXC) $(CXXFLAGS) -c -o $@ $<

clean:
	@rm -f test test.o Space.o Function.o

