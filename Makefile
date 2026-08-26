CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L
CPPFLAGS = -Iinclude
LDLIBS = -lm

TARGET = droneCloud
DEBUG_TARGET = droneCloud-debug
SOURCES = $(shell find src -type f -name '*.c')
LIB_SOURCES = $(filter-out src/main/main.c,$(SOURCES))
TEST_SOURCES = $(wildcard test/test_*.c)
TEST_TARGETS = $(patsubst test/%.c,bin/%,$(TEST_SOURCES))

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) $(LDLIBS) -o $(TARGET)

debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -O0 $(SOURCES) $(LDLIBS) -o $(DEBUG_TARGET)

test: $(TEST_TARGETS)
	@for test_program in $(TEST_TARGETS); do ./$$test_program; done

bin/test_%: test/test_%.c $(LIB_SOURCES) | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDLIBS) -o $@

bin:
	mkdir -p bin

gdb: debug
	gdb ./$(DEBUG_TARGET)

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(DEBUG_TARGET)

clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(TEST_TARGETS)

.PHONY: all debug test gdb valgrind clean
