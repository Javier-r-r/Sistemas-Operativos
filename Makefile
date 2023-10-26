EXECUTABLE=p1
CFLAGS= -Wall -g

p1: p1.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p1.c comand_list.c ficheros_list.c comand_list.h ficheros_list.h funcionesAux.c cabecerasAux.h

shell: p1.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p1.c comand_list.c ficheros_list.c comand_list.h ficheros_list.h funcionesAux.c cabecerasAux.h
	./p1
valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./${EXECUTABLE}