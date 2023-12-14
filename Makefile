EXECUTABLE=p3
CFLAGS= -Wall -g

p3: p3.o ficheros_list.o comand_list.o funcionesAux.o memory_list.o process_list.o
	gcc $(CFLAGS) -o $(EXECUTABLE) p3.o ficheros_list.o comand_list.o funcionesAux.o memory_list.o process_list.o

p3.o: p3.c cabecerasAux.h
	gcc $(CFLAGS) -c p3.c 

ficheros_list.o: ficheros_list.c ficheros_list.h
	gcc $(CFLAGS) -c ficheros_list.c

comand_list.o: comand_list.c comand_list.h
	gcc $(CFLAGS) -c comand_list.c

funcionesAux.o: funcionesAux.c cabecerasAux.h
	gcc $(CFLAGS) -c funcionesAux.c

memory_list.o: memory_list.c memory_list.h
	gcc $(CFLAGS) -c memory_list.c

process_list.o: process_list.c process_list.h
	gcc $(CFLAGS) -c process_list.c

limpiar: 
	rm *.o p3

valgrind: 
	valgrind --leak-check=full --show-reachable=yes ./$(EXECUTABLE)