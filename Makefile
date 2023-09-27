EXECUTABLE=p0
CFLAGS= -Wall -g

shell: p0.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p0.c comand_list.c comand_list.h

P0: P0.o
	gcc -Wall P0.c
valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./${EXECUTABLE}