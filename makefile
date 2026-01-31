TARGET = prog.out
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Werror -pedantic -std=gnu17 
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
# TEST
###########################################################################

UNIT_TEST_FOLDER = tests/unit
UNIT_TESTS_FILES =  $(wildcard $(UNIT_TEST_FOLDER)/*.c)
UNIT_TEST_OBJECTS = $(patsubst $(UNIT_TEST_FOLDER)/%.c, $(UNIT_TEST_FOLDER)/obj/%.o, $(UNIT_TESTS_FILES))
UNIT_TEST_EXECS = $(patsubst $(UNIT_TEST_FOLDER)/%.c, $(UNIT_TEST_FOLDER)/bin/%.out, $(UNIT_TESTS_FILES))

$(UNIT_TEST_FOLDER)/bin/%.out: $(UNIT_TEST_FOLDER)/%.c
	@ mkdir -p $(UNIT_TEST_FOLDER)/bin
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

test: $(UNIT_TEST_EXECS)
	@echo "Running tests..."
	@passCount=0; failCount=0; totalCount=0;\
    for test_exec in $(UNIT_TEST_EXECS); do \
        totalCount=$$((totalCount + 1)); \
        echo "Running $$test_exec "; \
        if $$test_exec; then \
            passCount=$$((passCount + 1)); \
        else \
            failCount=$$((failCount + 1)); \
        fi; \
		echo ""; \
    done; \
    echo "Unit tests completed. Total: $$totalCount, Passed: $$passCount, Failed: $$failCount."

###########################################################################

SAMPLE_TEST_FOLDER = tests/samples
SAMPLE_TEST_FILES =  $(wildcard $(SAMPLE_TEST_FOLDER)/*.c)
SAMPLE_TEST_OBJECTS = $(patsubst $(SAMPLE_TEST_FOLDER)/%.c, $(SAMPLE_TEST_FOLDER)/obj/%.o, $(SAMPLE_TEST_FILES))
SAMPLE_TEST_EXECS = $(patsubst $(SAMPLE_TEST_FOLDER)/%.c, $(SAMPLE_TEST_FOLDER)/bin/%.out, $(SAMPLE_TEST_FILES))

$(SAMPLE_TEST_FOLDER)/bin/%.out: $(SAMPLE_TEST_FOLDER)/%.c
	@ mkdir -p $(SAMPLE_TEST_FOLDER)/bin
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

sample: $(SAMPLE_TEST_EXECS)
	@echo "Running sample tests..."
	@for test_exec in $(SAMPLE_TEST_EXECS); do \
		echo "Running $$test_exec "; \
		$$test_exec; \
		echo ""; \
	done; \
	echo "Sample tests completed."

###########################################################################

.PHONY: clean

clean:
	-rm -f $(OBJ)/*.o
	-rm -f $(BIN)/$(TARGET)
	-rm -f $(TESTS)/output*.txt
	-rm -f $(UNIT_TEST_FOLDER)/bin/*.out
	-rm -f $(UNIT_TEST_FOLDER)/obj/*.o
	-rm -f $(SAMPLE_TEST_FOLDER)/bin/*.out
	-rm -f $(SAMPLE_TEST_FOLDER)/obj/*.o