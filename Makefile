
WIDTH=1280
HEIGHT=720

all: images
	gcc src/*.c -O2 -Wall -pedantic -std=gnu11 -Isrc/ -Iimg/ -o main -lSDL2

images: tools
	# uncomment me for images
	./concoord/concoord img/*.yaml

tools:
	make -C concoord

memtest:
	valgrind --track-origins=yes --leak-check=yes ./main
