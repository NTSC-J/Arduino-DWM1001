# Makefile for test binary, not for use with Arduino!

DEST := test
SRC := $(wildcard ../src/*.cpp src/*.cpp)
CXX := clang++
CXXFLAGS := -std=c++17 -g -Og -DDEBUG
INCLUDES = -I../src
LDFLAGS = -lserialport

.PHONY: clean

all: $(DEST)

$(DEST): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $^ -o $@
