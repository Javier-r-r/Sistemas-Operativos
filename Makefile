EXECUTABLE=p1
CFLAGS= -Wall -g

p0: p0.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p0.c comand_list.c ficheros_list.c comand_list.h ficheros_list.h funcionesAux.c cabecerasAux.h

shell: p0.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p0.c comand_list.c ficheros_list.c comand_list.h ficheros_list.h funcionesAux.c cabecerasAux.h
	./p0
P0: P0.o
	gcc -Wall P0.c
valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./${EXECUTABLE}