
all: images
	gcc src/main.c -Wall -pedantic -std=gnu11 -Iinclude/ -Iimg/ -o main -lSDL2

images: tools
	for FILE in $(shell find img/ | grep "\.pov"); do \
		tools/concoord -d img/ $$FILE; \
	done

tools: concoord

concoord:
	gcc tools/concoord.c -pedantic -std=gnu11 -Wall -o tools/concoord

run:
	./main

memtest:
	valgrind --track-origins=yes --leak-check=yes ./main
