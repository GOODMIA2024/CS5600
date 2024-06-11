CC = gcc
CFLAGS = -Iinclude -g
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = main

.PHONY: all clean LRU

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Compiled with default (Random Access) settings."
	rm -rf msg_store

LRU: CFLAGS += -DLRU=1
LRU: $(TARGET)
	@echo "Compiled with LRU settings."

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJ) msg_store