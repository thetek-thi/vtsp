CC     := gcc
CFLAGS := -Wall -Wextra -g

SRCFILES := $(wildcard src/*.c)
BINFILES := $(SRCFILES:src/%.c=bin/%)


all: mkdir $(BINFILES)

clean:
	rm -rf bin/

.PHONY: all clean


bin/%: src/%.c
	$(CC) $(CFLAGS) $< -o $@

mkdir:
	mkdir -p bin/
