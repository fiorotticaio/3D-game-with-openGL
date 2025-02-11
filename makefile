# Compiler used
CC = gcc
CXX = g++

# Compilation flags:
#  -g    Add debug information to the executable file
#  -Wall Turn on most, but not all, compiler warnings
CFLAGS  = -g -Wall
LINKING = -lglut -lGL -lGLU
TARGET = *
SVG_PATH = ./arena_teste_4.svg

all:
	$(CXX) $(CFLAGS) -o trabalhocg $(TARGET).cpp $(LINKING)

run: all
	./trabalhocg $(SVG_PATH)

run2: all
	./trabalhocg ./arena_teste_2.svg

run3: all
	./trabalhocg ./arena_teste_3.svg

run4: all
	./trabalhocg ../teste/arena_teste_4.svg

clean:
	$(RM) trabalhocg