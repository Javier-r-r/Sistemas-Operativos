
CFLAGS= -Wall -g

p2: practica.c comand_list.h ficheros_list.h cabecerasAux.h ficheros_list.o comand_list.o funcionesAux.o
	gcc $(CFLAGS) -o p2 practica.c ficheros_list.o comand_list.o funcionesAux.o

ficheros_list.o: ficheros_list.c ficheros_list.h
	gcc -c ficheros_list.c

comand_list.o: comand_list.c comand_list.h
	gcc -c comand_list.c

funcionesAux.o: funcionesAux.c cabecerasAux.h
	gcc -c funcionesAux.c

limpiar: 
	rm p2 comand_list.o ficheros_list.o funcionesAux.o

shell: p1.c
	gcc $(CFLAGS) -o $(EXECUTABLE) p1.c comand_list.c ficheros_list.c comand_list.h ficheros_list.h funcionesAux.c cabecerasAux.h
	./p1

valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./${EXECUTABLE}