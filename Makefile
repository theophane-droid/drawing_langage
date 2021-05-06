CC=gcc
SRC_FILES=src/draw.c src/liste.c src/error_handling.c src/language.c src/variable.c

all: build_ clean

build_: build $(SRC_FILES)
	$(CC) $(SRC_FILES) -o build/draw

build:
	mkdir build

clean:
	
remove:
	rm -rf build