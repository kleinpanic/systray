# Makefile for building the systray system info application

CC = gcc
CFLAGS = -Wall -Iinclude `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))
TARGET = systray

# Default target
all: $(TARGET)

# Compile each source file into the obj directory
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Clean up built files
clean:
	rm -rf obj $(TARGET)

