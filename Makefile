
all:
	gcc main.c -Wall -pedantic -std=c11 -o main -lSDL2

run:
	./main

memtest:
	valgrind --track-origins=yes --leak-check=yes ./main
