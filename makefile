CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

all: main.exe

main.exe: main.c
	$(CC) $(CFLAGS) -o $@ $<
