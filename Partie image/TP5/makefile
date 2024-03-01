CC=g++
CFLAGS=-Wall
BIN=./bin/

all: $(wildcard *.cpp)
	$(MAKE) $(patsubst %.cpp,%,$^)

.cpp:
	$(CC) $(CFLAGS) -o $(BIN)$@ $<

clean:
	rm -f $(BIN)*