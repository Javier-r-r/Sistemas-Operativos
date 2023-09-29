//Javier Rodriguez Rodriguez || j.rrodriguez1@udc.es
//Miguel Corton Deben || mcortond@udc.es

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
#include "comand_list.h"

int TrocearCadena(char * cadena, char * trozos[]){
  int i=1;
  if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    return 0;
  while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
  return i;
}

//Funcion que realiza el comando N de la lista hist
void Cmd_comand(tList commandList, tListF fileList, char *tr[]) {
  if (countItems(commandList) <= atoi(tr[0])) {
    printf("No hay tantos comandos en hist\n");
  } else {
    char *trozos[MAX/2];
    tItemL item = getItem(atoi(tr[0]), commandList);
    TrocearCadena(item.comando, trozos);
    procesar_comando(trozos, commandList, fileList);
  }
}

//Muestra el historial de comandos
void Cmd_hist(tList *commandList, char *tr[]){
  int ncmd;

  if(tr[0] == NULL){
    printList(*commandList);
  }else if(strcmp(tr[0],"-c") == 0){
    freeList(commandList);
  }else if (atoi(tr[0])){
    ncmd=(int) (abs(strtol(tr[0],NULL,10)));
    printUntilN(*commandList, ncmd);
  }
}

void Cmd_close (char *arg, tListF fileList)
{ 
    int df;
    
    if (arg==NULL || (df=atoi(arg))<0) { /*no hay parametro*/
        printListF(fileList);
        return;
    }

    
    if (close(df)==-1)
        perror("Inposible cerrar descriptor");
    else {
        removeElementF(df, &fileList);
        printf("Descriptor %d cerrado\n", df);
    }
}

void Cmd_open (char *arg, tListF fileList)
{
    int i,df, mode=0;
    
    if (arg==NULL) { /*no hay parametro*/
        printListF(fileList);
        return;
    }
    char *tr[MAX];
    TrocearCadena(arg, tr);
    for (i=1; tr[i]!=NULL; i++) {
      if (!strcmp(tr[i],"cr")) mode=O_CREAT;
      else if (!strcmp(tr[i],"ex")) mode=O_EXCL;
      else if (!strcmp(tr[i],"ro")) mode=O_RDONLY; 
      else if (!strcmp(tr[i],"wo")) mode=O_WRONLY;
      else if (!strcmp(tr[i],"rw")) mode=O_RDWR;
      else if (!strcmp(tr[i],"ap")) mode=O_APPEND;
      else if (!strcmp(tr[i],"tr")) mode=O_TRUNC; 
      else break;
    }
    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
      open(tr[0], mode, 0777);
      tItemF newItem;
      newItem.descriptor = df;
      newItem.mode = mode;
      strncpy(newItem.nombre, tr[0], MAX);
      insertElementF(newItem, &fileList);
      printf("Añadida entrada a la tabla ficheros abiertos: descriptor %d, modo %d, nombre %s\n", df, mode, tr[0]);
    }
}

//Imprime el pid del comando que se esta ejecutando el la red 
void Cmd_pid(char *tr[]) {
  if (tr[0] == NULL)
    printf("Process pid: %d \n", getpid());
  else if (strcmp(tr[0], "-p") == 0)
    printf("Parent process pid: %d \n", getppid());
}

//Funcion para obtener la fecha del sistema
void Cmd_date(char *tr[]) {
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
void Cmd_help(char *tr[])
{
  if(tr[0] == NULL){
    printf("-authors\n-pid\n-date\n-time\n-hist\n-comando\n-infosys\n-help\n-exit\n-quit\n-bye\n");
  }
  else if (!strcmp(tr[0],"authors")){
    printf("authors [-l][-n]: Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
  }
  else if (!strcmp(tr[0],"pid")){
    printf("pid [-p]: Prints the pid of the process executing the shell. pid -p rints the pid of the shell`s parent process.\n");
  }
  else if (!strcmp(tr[0],"date")){
    printf("date: It prints the current date in the format DD/MM/YYYY.");
  }
  else if (!strcmp(tr[0], "time")) {
    printf("time: It prints the current time in the format hh:mm:ss.");
  }
  else if (!strcmp(tr[0],"hist")){
    printf("hist [-c][-N]: Shows/clears the historic of commands executed by this shell.\n");
    printf("hist Prints all the commands that have been input with their order number\n");
    printf("hist -c Clears (empties) the list of historic commands\n");
    printf("hist -N prints the first N commands\n");
  }
  else if (!strcmp(tr[0],"comando")){
    printf("comand N: Repeats command number N (from historic list)\n");
  }
  else if (!strcmp(tr[0],"infosys")){
    printf("infosys: Prints information on the machine running the shell (as obtained via the uname system call/library function)\n");
  }
  else if (!strcmp(tr[0],"help")){
    printf("help [cmd]: displays a list of availables commands. help cmd gives a brief\n");
    printf("help on the usage of command cmd\n");
  }
  else if (!strcmp(tr[0],"bye")){
    printf("bye: Ends the shell\n");
  }
  else if (!strcmp(tr[0],"quit")){
    printf("quit: Ends the shell\n");
  }
  else if (!strcmp(tr[0],"exit")){
    printf("exit: Ends the shell\n");
  }
}

//Funcion para obtener la hora del sistema
void Cmd_time(char *tr[]) {
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
void Cmd_infosys(char *tr[]) {
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

void Cmd_chdir(char *tr[]){
    char dir[MAX];
    if (tr[0]==NULL)
        printf("%s \n", getcwd(dir,MAX));
    else if(chdir(tr[0]) == 0){
        printf("You changed of directory\n");
        printf("%s \n", getcwd(dir,MAX));
    }else if(chdir(tr[0])==-1){
        perror("Cannot change directory");
    }
}

//Funcion que maneja las opciones de la funcion Cmd_authors
void Cmd_authors(char *tr[]) {
  if (tr[0] == NULL) {
    printf("logins -> j.rrodriguez1@udc.es & m.cortond\n");
    printf("names -> Javier Rodriguez & Miguel Corton\n");
  } else if (strcmp(tr[0], "-l") == 0) {
    printf("logins -> j.rrodriguez1@udc.es & m.cortond\n");
  } else if (strcmp(tr[0], "-n") == 0) {
    printf("names -> Javier Rodriguez & Miguel Corton\n");
  } 
}

void Cmd_exit(tListF fileList, tList commandList){
    freeList(&commandList);
    freeListF(&fileList);
    free(commandList);
    free(fileList);
    exit(0);
}

struct cmd {
    char *nombre;
    void (*pfuncion) (char **);
};

struct cmd cmds[]={
  {"date",Cmd_date},
  {"time",Cmd_time},
  {"infosys",Cmd_infosys},
  {"authors",Cmd_authors},
  {"pid",Cmd_pid},
  {"chdir",Cmd_chdir},
  {"help",Cmd_help},
};

void procesar_comando(char *tr[], tList comandList, tListF fileList) {
  int i;
  if (tr[0] == NULL)
    return;
  if (!strcmp("comand", tr[0]))
    Cmd_comand(comandList, fileList, tr+1);
  else if (!strcmp("hist", tr[0]))
    Cmd_hist(&comandList, tr+1);
  else if (!strcmp("exit", tr[0]) || !strcmp("quit", tr[0]) || !strcmp("bye", tr[0]))
    Cmd_exit(fileList, comandList);
  else {
    for (i=0; cmds[i].nombre != NULL; i++){
      if (!strcmp(cmds[i].nombre, tr[0])) {
        (cmds[i].pfuncion) (tr+1);
        return;
      }
    }
    printf("No ejecutado\n");
  }
}

int main() {
  char comando[MAX]; // Usamos un array de caracteres para almacenar el comando
  char *tr[MAX];
  tList commandList;
  tListF fileList;
  createList(&commandList);
  createListF(&fileList);

  while (1) {
    printf("-> ");

    fgets(comando, MAX, stdin); // Leemos la entrada del usuario

    comando[strcspn(comando, "\n")] = '\0'; // Eliminamos el carácter de salto de línea

    if (comando[0] == '\0') //Si el usuario solo pulsa enter termina la funcion y vuelve al bucle
      continue;
    else {
      insertElement(comando, &commandList);
      TrocearCadena(comando, tr);
      procesar_comando(tr, commandList, fileList);
    }
  }

  return 0;
}
