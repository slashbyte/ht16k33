#makefile for DemoOfDoom
#Author: Slash/Byte
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp ht16k33.cpp starburst.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=demo_of_doom

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o demo_of_doom