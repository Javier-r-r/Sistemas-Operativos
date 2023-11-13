p2: practica.c comand_list.h ficheros_list.h cabecerasAux.h memory_list.h ficheros_list.o comand_list.o funcionesAux.o memory_list.o
	gcc -Wall -g -o p2 practica.c ficheros_list.o comand_list.o funcionesAux.o memory_list.o

ficheros_list.o: ficheros_list.c ficheros_list.h
	gcc -c ficheros_list.c

comand_list.o: comand_list.c comand_list.h
	gcc -c comand_list.c

funcionesAux.o: funcionesAux.c cabecerasAux.h
	gcc -c funcionesAux.c

memory_list.o: memory_list.c memory_list.h
	gcc -c memory_list.c

limpiar: 
	rm p2 comand_list.o ficheros_list.o funcionesAux.o memory_list.o

all: p2 funcionesAux.o comand_list.o ficheros_list.o memory_list.o
	./p2
	
valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./p2