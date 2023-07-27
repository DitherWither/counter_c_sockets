CFLAGS= -Wall -Wpedantic
CC=musl-clang
LDFLAGS=

.PHONY: all

all: build/server

.PHONY: clean

clean:
	rm -r build

.PHONY: init

init:
	mkdir -p build

build/server: build/main.o build/common.o build/connection.o build/router.o init
	$(CC) $(CFLAGS) build/main.o build/common.o build/connection.o build/router.o -o build/server $(LDFLAGS)

build/main.o: main.c init
	$(CC) $(CFLAGS) -o build/main.o -c main.c

build/common.o: common.c init
	$(CC) $(CFLAGS) -o build/common.o -c common.c

build/connection.o: connection.c init
	$(CC) $(CFLAGS) -o build/connection.o -c connection.c

build/router.o: router.c
	$(CC) $(CFLAGS) -o build/router.o -c router.c


