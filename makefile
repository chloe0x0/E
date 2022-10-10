CC = gcc
CFLAGS = -Wall -Wextra -O2 -o E
REMOVE = del

all:
	$(CC) $(CFLAGS) ./src/E.c

clean:
	$(REMOVE) E.exe