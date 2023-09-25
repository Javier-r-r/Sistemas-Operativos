//Javier Rodriguez Rodriguez || j.rrodriguez1@udc.es


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdbool.h>
#include "head_list.h"

//Commprueba si el número está en el historial
bool isNumberHist(char* str){
  for (int j=0; j < strlen(str) ; j++){
    if(str[j]<48 || str[j]>57){
         
      if(str[j] != 45 && j>0){ //45 is "-" sign in ASCII table
        return false;
      }
      if(j>0 && str[j]==45) return false; //avoid negative numbers with minus sign within it
    }
  }
  return true;
}

//Funcion que realiza el comando N de la lista hist
void Cmd_comand(tList commandList, int N) {

  tList aux = commandList;
  while (aux != NULL) {
    if (aux->data.index == N) {
      procesar_comando(strtok(aux->data.comando, " "), &commandList);
      break;
    }
    aux = aux -> next;
  }

}

//Función para añadir comandos al historial
void addCommand(tList commandList, char *command) {
  if (command != NULL) {
    insertElement(command, &commandList);
  }
}

//Muestra el historial de comandos
void Cmd_hist(tList *commandList, char *arg){
  int ncmd;

  if(arg == NULL){
    printList(*commandList);
  }else if(strcmp(arg,"-c") == 0){
    freeList(commandList);
  }else if (atoi(arg)){
    if(isNumberHist(arg)){
      ncmd=(int) abs(strtol(arg,NULL,10));
      printUntilN(*commandList, ncmd);
    } else printf("Please input a valid number for this command\n");
  }
}

//Imprime el pid del comando que se esta ejecutando el la red 
void Cmd_pid(char *arg) {
  if (arg == NULL)
    printf("Process pid: %d \n", getpid());
  else if (strcmp(arg, "-p") == 0)
    printf("Parent process pid: %d \n", getppid());
}

//Funcion para obtener la fecha del sistema
void Cmd_date() {
  // Obtener la fecha actual
  time_t t;
  struct tm *tm_info;

  time(&t);
  tm_info = localtime(&t);

  // Formatear la fecha en el formato DD/MM/YYYY
  char fecha[11]; // Espacio suficiente para almacenar "DD/MM/YYYY\0"
  strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm_info);

  // Imprimir la fecha en el formato correcto
  printf("Fecha actual: %s\n", fecha);
}

//Imprime información sobre el comando que se le pasa, si no pasa comando muestra por pantalla los comandos disponibles
void Cmd_help(char *arg)
{
  if(arg == NULL){
    printf("-authors\n-pid\n-date\n-time\n-hist\n-comando\n-infosys\n-help\n-exit\n-quit\n-bye\n");
  }
  else if (!strcmp(arg,"authors")){
    printf("authors [-l][-n]: Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
  }
  else if (!strcmp(arg,"pid")){
    printf("pid [-p]: Prints the pid of the process executing the shell. pid -p rints the pid of the shell`s parent process.\n");
  }
  else if (!strcmp(arg,"date")){
    printf("date: It prints the current date in the format DD/MM/YYYY.");
  }
  else if (!strcmp(arg, "time")) {
    printf("time: It prints the current time in the format hh:mm:ss.");
  }
  else if (!strcmp(arg,"hist")){
    printf("hist [-c][-N]: Shows/clears the historic of commands executed by this shell.\n");
    printf("hist Prints all the commands that have been input with their order number\n");
    printf("hist -c Clears (empties) the list of historic commands\n");
    printf("hist -N prints the first N commands\n");
  }
  else if (!strcmp(arg,"comando")){
    printf("comando N: Repeats command number N (from historic list)\n");
  }
  else if (!strcmp(arg,"infosys")){
    printf("infosys: Prints information on the machine running the shell (as obtained via the uname system call/library function)\n");
  }
  else if (!strcmp(arg,"help")){
    printf("help [cmd]: displays a list of availables commands. help cmd gives a brief\n");
    printf("help on the usage of command cmd\n");
  }
  else if (!strcmp(arg,"bye")){
    printf("bye: Ends the shell\n");
  }
  else if (!strcmp(arg,"quit")){
    printf("quit: Ends the shell\n");
  }
  else if (!strcmp(arg,"exit")){
    printf("exit: Ends the shell\n");
  }
}

//Funcion para obtener la hora del sistema
void Cmd_time() {
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
void Cmd_infosys() {
  struct utsname Cmd_infosys;

  if (uname(&Cmd_infosys) == -1) {
    perror("uname");
    return;
  }

  printf("System Information:\n");
  printf("  Operating System: %s\n", Cmd_infosys.sysname);
  printf("  Node Name: %s\n", Cmd_infosys.nodename);
  printf("  Release: %s\n", Cmd_infosys.release);
  printf("  Version: %s\n", Cmd_infosys.version);
  printf("  Machine: %s\n", Cmd_infosys.machine);
}

//Funcion que maneja las opciones de la funcion Cmd_authors
void Cmd_authors(char *arg) {
  if (arg == NULL) {
    printf("logins -> j.rrodriguez1@udc.es & m.cortond\n");
    printf("names -> Javier Rodriguez & Miguel Corton\n");
  } else if (strcmp(arg, "-l") == 0) {
    printf("logins -> j.rrodriguez1@udc.es & m.cortond\n");
  } else if (strcmp(arg, "-n") == 0) {
    printf("names -> Javier Rodriguez & Miguel Corton\n");
  } 
}

//Funcion encargada de llamar a la funcion correspondiente
void procesar_comando(char *comando, tList *commandList) {
  if (comando[0] == '\0') { //Si el usuario solo pulsa enter termina la funcion y vuelve al bucle
    return;
  } else {
    addCommand(*commandList, comando);
    char *comand = strtok(comando, " ");
    char *arg = strtok(NULL, " ");
    if (strcmp(comand, "exit") == 0 ||strcmp(comand, "quit") == 0 || strcmp(comand, "bye") == 0)
      exit(0);
    else if (strcmp(comand, "date") == 0)
      Cmd_date();
    else if(strcmp(comand, "time") == 0)
      Cmd_time();
    else if (strcmp(comand, "infosys") == 0)
      Cmd_infosys();
    else if (strcmp(comand, "authors") == 0)
      Cmd_authors(arg);
    else if (strcmp(comand, "comand") == 0)
      Cmd_comand(*commandList, atoi(arg));
    else if (strcmp(comand, "hist") == 0)
      Cmd_hist(commandList, arg);
    else if(strcmp(comand, "pid") == 0)
      Cmd_pid(arg);
  }
}

int main() {
  char comando[256]; // Usamos un array de caracteres para almacenar el comando
  tList commandList;
  createList(&commandList);

  while (1) {
    printf("> ");

    gets(comando); // Leemos la entrada del usuario

    comando[strcspn(comando, "\n")] = '\0'; // Eliminamos el carácter de salto de línea

    procesar_comando(comando, &commandList);
  }

  freeList(&commandList); //Vacia la lista de comandos antes de terminar el shell

  return 0;
}
