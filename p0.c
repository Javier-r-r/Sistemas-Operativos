#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/utsname.h>

struct command {
  int number;
  char *command;
  struct command *next;
};

//Función para añadir comandos al historial
void addCommand(struct command **commandList, int *commandCount, const char *command) {
  struct command *newCommand = (struct command *)malloc(sizeof(struct command));
  if (newCommand != NULL) {
    (*commandCount)++;
    newCommand->number = (*commandCount);
    newCommand->command = strdup(command); // Copia la cadena de comandos
    newCommand->next = *commandList;
    *commandList = newCommand;
  }
}


//Función para imprimir los contenidos de la lista de comandos
void printCommands(struct command *commandList) {
  struct command *current = commandList;

  printf("Lista de comandos:\n");
  while (current != NULL) {
    printf("%d: %s\n", current -> number, current -> command);
    current = current -> next;
  }
}

//Funcion para vaciar la lista de comandos del historial
void clearCommands(struct command* commandList) {
  while(commandList != NULL) {
    struct command* aux = commandList;
    commandList = commandList -> next;
    free(aux -> command);
    free(aux);
  }
}

//Funcion para obtener la fecha del sistema
void date() {
  // Obtener la fecha actual
  time_t t;
  struct tm *tm_info;

  time(&t);
  tm_info = localtime(&t);

  // Formatear la fecha en el formato DD/MM/YYYY
  char fecha[11]; // Espacio suficiente para almacenar "DD/MM/YYYY\0"
  strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm_info);

  // Imprimir la fecha formateada
  printf("Fecha actual: %s\n", fecha);
}

//Funcion para obtener la hora del sistema
void times() {
  // Obtener la hora actual
  time_t t;
  struct tm *tm_info;

  time(&t);
  tm_info = localtime(&t);

  // Formatear la hora en el formato hh:mm:ss
  char hora[9]; // Espacio suficiente para almacenar "hh:mm:ss\0"
  strftime(hora, sizeof(hora), "%H:%M:%S", tm_info);

  // Imprimir la hora formateada
  printf("Hora actual: %s\n", hora);
}

//Funcion para imprimir por pantalla la informacion del sistema
void systemInfo() {
  struct utsname systemInfo;

  if (uname(&systemInfo) == -1) {
    perror("uname");
    return;
  }

  printf("System Information:\n");
  printf("  Operating System: %s\n", systemInfo.sysname);
  printf("  Node Name: %s\n", systemInfo.nodename);
  printf("  Release: %s\n", systemInfo.release);
  printf("  Version: %s\n", systemInfo.version);
  printf("  Machine: %s\n", systemInfo.machine);
}

//Funcion encargada de llamar a la funcion correspondiente
void procesar_comando(const char *comando, int *terminado, struct command* commandList) {
  if (strcmp(comando, "exit") == 0 ||strcmp(comando, "quit") == 0 || strcmp(comando, "bye") == 0)
    *terminado = 0;
  else if (strcmp(comando, "date") == 0)
    date();
  else if(strcmp(comando, "time") == 0)
    times();
  else if (strcmp(comando, "infosys") == 0)
    systemInfo();
  else if (strcmp(comando, "hist") == 0)
    printCommands(commandList);
}

int main() {
  char comando[256]; // Usamos un array de caracteres para almacenar el comando
  int terminado = 1;
  struct command* commandList = NULL;
  int commandCount = 0;

  while (terminado) {
    printf("> ");

    gets(comando); // Leemos la entrada del usuario
    comando[strcspn(comando, "\n")] = '\0'; // Eliminamos el carácter de salto de línea

    addCommand(&commandList, &commandCount, comando);
    procesar_comando(comando, &terminado, commandList);
  }

  clearCommands(commandList); //Vacia la lista de comandos antes de terminar el shell

  return 0;
}
