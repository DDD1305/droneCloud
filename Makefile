CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

TARGET = droneCloud
DEBUG_TARGET = droneCloud-debug
SOURCES = $(shell find src -type f -name '*.c')

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -g -O0 $(SOURCES) -o $(DEBUG_TARGET)

gdb: debug
	gdb ./$(DEBUG_TARGET)

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(DEBUG_TARGET)

clean:
	rm -f $(TARGET) $(DEBUG_TARGET)

.PHONY: all debug gdb valgrind clean
