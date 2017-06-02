
all:
	gcc main.c -o main -lSDL2

memtest:
	valgrind --track-origins=yes --leak-check=yes --show-reachable=yes ./main
