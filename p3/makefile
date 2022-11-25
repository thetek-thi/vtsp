CC     := gcc
CFLAGS := -Wall -Wextra -g


all: reader-writer

clean:
	rm -rf bin/

.PHONY: all clean


reader-writer: mkdir
	$(CC) $(CFLAGS) src/reader-writer/reader-writer.c -o bin/reader-writer

mkdir:
	mkdir -p bin/
