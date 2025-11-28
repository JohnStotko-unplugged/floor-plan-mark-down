TARGET = prog.out
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
SRC = src
BIN = bin
OBJ = obj

.PHONY: default all clean

default: $(BIN)/$(TARGET)
all: default

# For every *.c, objects has a *.o entry
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))

# All the header files
HEADERS = $(wildcard *.h)

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS)
	@ mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(BIN)/$(TARGET): $(OBJECTS)
	@ mkdir -p $(BIN)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)