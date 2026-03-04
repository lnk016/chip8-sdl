CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LDFLAGS = -lSDL3

SRC = src/*.c
OUT = main

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
