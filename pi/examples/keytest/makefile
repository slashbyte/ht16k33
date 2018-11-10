#makefile for keyTest demo
#Author: Slash/byte
CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lwiringPi
SOURCES=main.cpp ht16k33.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=keyTest

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o keyTest