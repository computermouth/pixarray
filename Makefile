
WIDTH=1280
HEIGHT=720

all: images
	gcc src/main.c -O3 -Wall -pedantic -std=gnu11 -Iinclude/ -Iimg/ -o main -lSDL2

images: tools
	for FILE in $(shell find img/ | grep "\.pov"); do \
		tools/concoord -d img/ -h $(HEIGHT) $$FILE || exit 1 ; \
	done

tools: concoord

concoord:
	gcc tools/concoord.c -pedantic -std=gnu11 -Wall -o tools/concoord

run:
	./main

memtest:
	valgrind --track-origins=yes --leak-check=yes ./main
