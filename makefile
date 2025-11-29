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
# E2E TESTS
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
# UNIT TESTS
###########################################################################

UNIT_TEST_FOLDER = tests/unit
UNIT_TESTS_FILES =  $(wildcard $(UNIT_TEST_FOLDER)/*.c)
UNIT_TEST_OBJECTS = $(patsubst $(UNIT_TEST_FOLDER)/%.c, $(UNIT_TEST_FOLDER)/obj/%.o, $(UNIT_TESTS_FILES))
UNIT_TEST_EXECS = $(patsubst $(UNIT_TEST_FOLDER)/%.c, $(UNIT_TEST_FOLDER)/bin/%.out, $(UNIT_TESTS_FILES))

$(UNIT_TEST_FOLDER)/bin/%.out: $(UNIT_TEST_FOLDER)/%.c
	@ mkdir -p $(UNIT_TEST_FOLDER)/bin
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

unit-tests: $(UNIT_TEST_EXECS)
	@echo "Running tests..."
	@passCount=0; failCount=0; totalCount=0;\
    for test_exec in $(UNIT_TEST_EXECS); do \
        totalCount=$$((totalCount + 1)); \
        echo "Running $$test_exec..."; \
        if $$test_exec; then \
            passCount=$$((passCount + 1)); \
        else \
            failCount=$$((failCount + 1)); \
        fi; \
    done; \
    echo "Unit tests completed. Total: $$totalCount, Passed: $$passCount, Failed: $$failCount."

###########################################################################



.PHONY: clean

clean:
	-rm -f $(OBJ)/*.o
	-rm -f $(BIN)/$(TARGET)
	-rm -f $(TESTS)/output*.txt
	-rm -f $(UNIT_TEST_FOLDER)/bin/*.out