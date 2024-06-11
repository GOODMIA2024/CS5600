CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = cipher.c polybius.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = cipher

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)