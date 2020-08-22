
WIDTH=1280
HEIGHT=720

all: images
	gcc src/*.c img/*.c -O2 -Wall -pedantic -std=gnu11 -Isrc/ -Iimg/ -flto -o main -lSDL2
	strip main

images: tools
	# uncomment me for images
	./concoord/concoord img/*.yaml

tools:
	make -C concoord

memtest:
	valgrind --track-origins=yes --leak-check=yes ./main
