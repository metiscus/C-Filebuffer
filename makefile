CFLAGS=-Wall -Wextra
CC=gcc

all: libfilebuffer.a filebuffer_test

libfilebuffer.a: filebuffer.o filebuffer.c
	ar -rcs libfilebuffer.a filebuffer.o

.PHONY: tests
tests: filebuffer_test
	@echo "Running unit tests"
	@./filebuffer_test
	@echo "All tests passed"

filebuffer_test: filebuffer_test.o filebuffer_test.c libfilebuffer.a
	$(CC) $(CFLAGS) -o filebuffer_test filebuffer_test.o -L. -lfilebuffer

clean:
	-rm -f filebuffer.o libfilebuffer.a filebuffer_test filebuffer_test.o
