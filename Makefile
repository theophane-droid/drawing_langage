CC=gcc

all: build_ clean

build_: build src/draw.c src/liste.c
	$(CC) src/draw.c src/liste.c -o build/draw

build:
	mkdir build

clean:
	
remove:
	rm -rf build