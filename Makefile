
WIDTH=1280
HEIGHT=720

all: images
	gcc src/ww.c src/main.c src/files.c src/mpack.c -O3 -Wall -pedantic -std=gnu11 -Isrc/ -Iimg/ -o main -lSDL2

images: tools
	./concoord/concoord img/*.yaml

tools:
	make -C concoord

memtest:
	valgrind --track-origins=yes --leak-check=yes ./main
