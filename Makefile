OBJ = a.out

p0: p0.o
	gcc -Wall p0.c
run:
	./${OBJ}
valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./${OBJ}