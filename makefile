# Compiler used
CC = gcc
CXX = g++

# Compilation flags:
#  -g    Add debug information to the executable file
#  -Wall Turn on most, but not all, compiler warnings
CFLAGS  = -g -Wall
LINKING = -lglut -lGL -lGLU
TARGET = *
SVG_STANDARD_PATH = ./arena_teste.svg
SVG_OPPONENT_TESTING_PATH = ./arena_unique_opponent.svg

all:
	$(CXX) $(CFLAGS) -o trabalhocg $(TARGET).cpp $(LINKING)

run: all
	./trabalhocg $(SVG_STANDARD_PATH)

opponent: all
	./trabalhocg $(SVG_OPPONENT_TESTING_PATH)

run2: all
	./trabalhocg ./arena_teste_2.svg

run3: all
	./trabalhocg ./arena_teste_3.svg

run4: all
	./trabalhocg ../teste/arena_teste_4.svg

clean:
	$(RM) trabalhocg