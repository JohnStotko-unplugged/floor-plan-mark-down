TARGET = prog.out
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
SRC = src
BIN = bin
OBJ = obj
TESTS = tests

.PHONY: default all

default: $(BIN)/$(TARGET)
all: default

###########################################################################
# BUILDING THE PROGRAM
###########################################################################

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

###########################################################################
# TESTS
###########################################################################

.PHONY: test

test: $(BIN)/$(TARGET)
	@echo "Running tests..."
	@for input in $(wildcard $(TESTS)/input*.txt); do \
        num=$$(echo $$input | sed 's/.*input\([0-9]*\).txt/\1/'); \
        output="$(TESTS)/output$${num}.txt"; \
        ./$(BIN)/$(TARGET) < $$input > $$output || (echo "program failed"; exit 1); \
        expected="$(TESTS)/expected$${num}.txt"; \
        echo "Comparing $$output to $$expected..."; \
        diff -u $$expected $$output || (echo "Test $$num failed"; exit 1); \
    done
	@echo "All tests passed."


###########################################################################

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f $(BIN)/$(TARGET)
	-rm -f $(TESTS)/output*.txt