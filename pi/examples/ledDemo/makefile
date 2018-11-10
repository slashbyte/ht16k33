#makefile for scrolling text demo
#Author: Slash/byte
CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lwiringPi
SOURCES=main.cpp ht16k33.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=demo

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o demo