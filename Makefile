# Makefile for test binary, not for use with Arduino!

DEST := test
CXX ?= clang++
CXXFLAGS = -std=c++17 -g -Og -DDEBUG
LDFLAGS = -lserialport

.PHONY: clean

all: $(DEST)

$(DEST): src/DWM1001.cpp src/SerialDWM1001.cpp src/SerialTest.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

