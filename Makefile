
all:
	gcc main.c -Wall -std=c11 -o main -lSDL2

memtest:
	valgrind --track-origins=yes --leak-check=yes --show-reachable=yes ./main
