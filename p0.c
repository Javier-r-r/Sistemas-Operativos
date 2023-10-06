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
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h>
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
<<<<<<< HEAD
void Cmd_comand(char *tr[], tList commandList, tListF fileList) {
  int N = atoi(tr[0]);
  if (countItems(commandList) <= N) {
    printf("No hay tantos comandos en hist\n");
  } else {
    char *trozos[MAX/2];
    tItemL item = getItem(N, commandList);
    TrocearCadena(item.comando,trozos);
    procesar_comando(trozos, &commandList, &fileList);
  }
}

//Muestra el historial de comandos
=======
void Cmd_comand(tList commandList, tListF fileList, char *tr[]) {
  int N = atoi (tr[0]);
  if (N) {
    if (countItems(commandList) <= N) {
      printf("No hay tantos comandos en hist\n");
    } else {
      char *trozos[MAX/2];
      tItemL item = getItem(atoi(tr[0]), commandList);
      TrocearCadena(item.comando, trozos);
      printf("Ejecutando hist (%d): %s\n", atoi(tr[0]), trozos[0]);
      procesar_comando(trozos, commandList, fileList);
    }
  } else 
    printf("Comando no encontrado\n");
}

//Muestra el historial de comandos, empieza enumerando por 1
>>>>>>> definitivo
void Cmd_hist(tList *commandList, char *tr[]){
  int ncmd;

  if(tr[0] == NULL){
    printList(*commandList);
  }else if(strcmp(tr[0],"-c") == 0){
    freeList(commandList);
  }else if (atoi(tr[0])){
    ncmd=(int) (abs(strtol(tr[0],NULL,10)));
    printUntilN(*commandList, ncmd);
  } else
    printf("Opcion no encontrada\n");
}

void Cmd_close (char *tr[], tListF fileList) { 
  int df = atoi(tr[0]);
    
  if (tr[0]==NULL || df<0) { /*no hay parametro*/
    printListF(fileList);
    return;
  }
  
  if (close(df)!=-1)
    removeElementF(df, &fileList);
}

void Cmd_open (char *tr[], tListF fileList) {
  int i,df, mode=0;
    
  if (tr[0]==NULL) { /*no hay parametro*/
    printListF(fileList);
    return;
  }
  for (i=1; tr[i]!=NULL; i++) {
    if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
    else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
    else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY; 
    else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
    else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
    else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
    else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC; 
    else break;
  }
  df=open(tr[0],mode,0777);
  if (df==-1)
    perror ("Imposible abrir fichero");
  else{
    tItemF newItem;
    newItem.descriptor = df;
    newItem.mode = mode;
    strncpy(newItem.nombre, tr[0], MAX);
    insertElementF(newItem, &fileList);
    printf("Añadida entrada %d a la tabla ficheros abiertos\n", df);
  }
}

void Cmd_dup (char *tr[], tListF fileList) { 
  int df, duplicado;
  char aux[MAX],*p;
    
  if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
    printListF(fileList);                 /*o el descriptor es menor que 0*/
    return;
  }
 
  p=NombreFicheroDescriptor(df, fileList);

  sprintf (aux,"dup %d (%s)",df, p);
  if ((duplicado = fcntl(df, F_DUPFD, 0)) == -1)
    perror("Error al dulpicar descriptor");
  else {
    tItemF newItem;
    newItem = getItemF(p, fileList);
    insertElementF(newItem, &fileList);
  }
}

void Cmd_listopen(tListF fileList) {
  printListF(fileList);
}

//Imprime el pid del comando que se esta ejecutando el la red 
void Cmd_pid(char *tr[]) {
  if (tr[0] == NULL)
<<<<<<< HEAD
    printf("Pid del shell: %d \n", getpid());
  else if (strcmp(tr[0], "-p") == 0)
    printf("Pid del padre del shell: %d \n", getppid());
=======
    printf("Process pid: %d \n", getpid());
  else if (strcmp(tr[0], "-p") == 0)
    printf("Parent process pid: %d \n", getppid());
  else
    printf("Opcion no encontrada\n");
>>>>>>> definitivo
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

<<<<<<< HEAD
  // Imprimir la fecha en el formato correcto
  printf("%s\n", fecha);
=======
    // Imprimir la fecha en el formato correcto
    printf("%s\n", fecha);
>>>>>>> definitivo
}

//Imprime información sobre el comando que se le pasa, si no pasa comando muestra por pantalla los comandos disponibles
void Cmd_help(char *tr[])
{
  if(tr[0] == NULL){
<<<<<<< HEAD
    printf("Comandos disponibles:\n-authors\n-pid\n-date\n-time\n-hist\n-comando\n-infosys\n-help\n-exit\n-quit\n-bye\n");
  }
  else if (!strcmp(tr[0],"authors")){
    printf("authors [-l][-n]: Muestra los nombres y/o los logins de los autores\n");
  }
  else if (!strcmp(tr[0],"pid")){
    printf("pid [-p]: Muestra el pid del shell o su proceso padre\n");
=======
    printf("Ayuda sobre los comandos, comandos disponilbes:\n-authors\n-pid\n-date\n-time\n-hist\n-comand\n-infosys\n-help\n-exit\n-quit\n-bye\n");
  }
  else if (!strcmp(tr[0],"authors")){
    printf("authors [-l][-n]: Muestra los nombres y/o logins de los autores\n");
  }
  else if (!strcmp(tr[0],"pid")){
    printf("pid [-p]: Muestra el pid del shell o de su proceso padre\n");
>>>>>>> definitivo
  }
  else if (!strcmp(tr[0],"date")){
    printf("date: Muestra la fecha actual\n");
  }
  else if (!strcmp(tr[0], "time")) {
    printf("time: Muestra la hora actual\n");
  }
  else if (!strcmp(tr[0],"hist")){
    printf("hist [-c][-N]: Muestra (o borra) el historico de comandos\n");
<<<<<<< HEAD
    printf("\t-N: Muestra los N primeros comandos\n");
    printf("\t-c: Borra el historico\n");
  }
  else if (!strcmp(tr[0],"comand")){
    printf("comand [-N]: Repite el comando N (del historico)\n");
  }
  else if (!strcmp(tr[0],"infosys")){
    printf("infosys: Muestra la informacion de la maquina donde se ejecuta el shell\n");
  }
  else if (!strcmp(tr[0],"help")){
    printf("help [cmd]: Muestra ayuda sobre los comandos\n");
    printf("\tcmd: info sobre el comando cmd\n");
  }
  else if (!strcmp(tr[0], "chdir")) {
    printf("chdir [dir]: Cambia (o muestra) el directorio actual del shell\n");
  }
  else if (!strcmp(tr[0], "listopen")) {
    printf("listopen [n]: Lista los ficheros abiertos (al menos n) del shell\n");
  }
  else if (!strcmp(tr[0], "open")) {
    printf("open fich m1,m2...: Abre el fichero fich, y lo anade a la lista de ficheros abiertos del shell\n");
    printf("m1, m2... es el modo de apertura (or bit a bit de los siguientes):\n");
    printf("\tcr: O_CREAT\tap: O_APPEND\n\tex: O_EXCL\tro: O_RDONLY\n\trw: O_RDWR\two: O_WRONLY\n\ttr: O_TRUNC\n");
  }
  else if (!strcmp(tr[0], "close")) {
    printf("close df: Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n");
  }
  else if (!strcmp(tr[0], "dup")) {
    printf("dup df: Duplica el descriptor de fichero df y anade una nueva entrada a la lista de ficheros abiertos\n");
  }
  else if (!strcmp(tr[0],"bye")){
    printf("bye: Termina la ejecucion del shell\n");
  }
  else if (!strcmp(tr[0],"quit")){
    printf("quit: Termina la ejecucion del shell\n");
  }
  else if (!strcmp(tr[0],"exit")){
    printf("exit: Termina la ejecucion del shell\n");
  } else
    printf("%s no encontrado", tr[0]);
=======
    printf("\t-c: borra el historico\n");
    printf("\tN: Muestra los primeros N comandos\n");
  }
  else if (!strcmp(tr[0],"comand")){
    printf("comand -N: Repite el comando N (del historico)\n");
  }
  else if (!strcmp(tr[0],"infosys")){
    printf("infosys: Muestra informacion de la maquina donde se ejcuta el shell\n");
  }
  else if (!strcmp(tr[0],"help")){
    printf("help [cmd]: Muestra una lista de los comandos disponibles\n");
    printf("\tcmd: Muestra ayuda sobre el comando cmd\n");
  }
  else if (!strcmp(tr[0],"bye") || !(strcmp(tr[0], "quit")) || !strcmp(tr[0], "exit")){
    printf("%s: Termina la ejecucion del shell\n", tr[0]);
  }
  else if (!strcmp(tr[0], "open")) {
    printf("open [fich] [m1 m2...]  Abre el fichero fich y lo anade a la lista de ficheros abiertos del shell\n");
    printf("\tm1,m2... es el modo de apertura (or bit a bit de los siguientes):\n");
    printf("\tcr: O_CREAT\tap: O_APPEND\n\tew: O_EXCL\tro: O_RDONLY\n\tre: O_RDRW\two: O_WRONLY\n\ttr: O_TRUNC\n");
  }
  else if (!strcmp(tr[0], "close")) {
    printf("close df: Cierra el descripor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n");
  }
  else if (!strcmp(tr[0], "dup")){
    printf("dup df: Duplica el descriptor de fichero df y anade una nueva entrada a la lista de ficheros abiertos\n");
  }
  else if (!strcmp(tr[0], "listopen")){
    printf("listopen [n]; Lista los ficheros abiertos (al menos n) del shell\n");
  }
  else
    printf("%s no encontrado\n", tr[0]);
>>>>>>> definitivo
}

//Funcion para obtener la hora del sistema
void Cmd_time(char *tr[]) {
  if (tr[0] == NULL) {
    // Obtener la hora actual
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    // Formatear la hora en el formato hh:mm:ss
    char hora[9]; // Espacio suficiente para almacenar "hh:mm:ss\0"
    strftime(hora, sizeof(hora), "%H:%M:%S", tm_info);

<<<<<<< HEAD
  // Imprimir la hora formateada
  printf("%s\n", hora);
=======
    // Imprimir la hora formateada
    printf("%s\n", hora);
  } else
    printf("Opcion no encontrada\n");
>>>>>>> definitivo
}

//Funcion para imprimir por pantalla la informacion del sistema
void Cmd_infosys(char *tr[]) {
  if (tr[0] == NULL) {
    struct utsname Cmd_infosys;

    if (uname(&Cmd_infosys) == -1) {
      perror("uname\n");
      return;
    }

    printf("System Information:\n");
    printf("  Operating System: %s\n", Cmd_infosys.sysname);
    printf("  Node Name: %s\n", Cmd_infosys.nodename);
    printf("  Release: %s\n", Cmd_infosys.release);
    printf("  Version: %s\n", Cmd_infosys.version);
    printf("  Machine: %s\n", Cmd_infosys.machine);
  } else
    printf("Opcion no encontrada\n");
}

void Cmd_chdir(char *tr[]){
    char dir[MAX];
    if (tr[0]==NULL)
        printf("%s \n", getcwd(dir,MAX));
    else if(chdir(tr[0]) == 0){
<<<<<<< HEAD
        printf("%s \n", getcwd(dir,MAX));
    }else if(chdir(tr[0])==-1){
        perror("Imposible cambiar de directorio\n");
=======
        printf("You changed of directory\n");
        printf("%s \n", getcwd(dir,MAX));
    }else if(chdir(tr[0])==-1){
        perror("Cannot change directory\n");
>>>>>>> definitivo
    }
}

//Funcion que maneja las opciones de la funcion Cmd_authors
void Cmd_authors(char *tr[]) {
  if (tr[0] == NULL) {
<<<<<<< HEAD
    printf("Javier: j.rrodriguez1@udc.es\n");
    printf("Miguel: m.cortond\n");
  } else if (strcmp(tr[0], "-l") == 0) {
    printf("j.rrodriguez1@udc.es\nm.cortond\n");
  } else if (strcmp(tr[0], "-n") == 0) {
    printf("Javier Rodriguez\nMiguel Corton\n");
  } 
=======
    printf("logins -> j.rrodriguez1@udc.es & m.cortond\n");
    printf("names -> Javier Rodriguez & Miguel Corton\n");
  } else if (strcmp(tr[0], "-l") == 0) {
    printf("logins -> j.rrodriguez1@udc.es & m.cortond\n");
  } else if (strcmp(tr[0], "-n") == 0) {
    printf("names -> Javier Rodriguez & Miguel Corton\n");
  } else
    printf("Opcion no encontrada\n");
>>>>>>> definitivo
}

void Cmd_exit(tListF fileList, tList commandList){
    freeList(&commandList);
    freeListF(&fileList);
    free(commandList);
    free(fileList);
    exit(0);
<<<<<<< HEAD
=======
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
    printf("%s no encontrado. Consulte la lista de comandos disponibles con help\n", tr[0]);
  }
>>>>>>> definitivo
}

struct cmd {
    char *nombre;
    void (*pfuncion) (char **)
};

struct cmd cmds[]={
        {"date",Cmd_date},
        {"time",Cmd_time},
        {"infosys",Cmd_infosys},
        {"authors",Cmd_authors},
        {"pid",Cmd_pid},
        {"chdir",Cmd_chdir},
        {"help",Cmd_help},
        {"exit", Cmd_exit},
        {"quit", Cmd_exit},
        {"bye", Cmd_exit},
};

void procesar_comando(char *tr[], tList *comandList, tListF *fileList) {
  int i;
  if (tr[0] == NULL)
    return;
  if (!strcmp("comand", tr[0]))
    Cmd_comand(tr+1, *comandList, *fileList);
  else if (!strcmp("hist", tr[0]))
    Cmd_hist(comandList, tr+1);
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

int main(int argc, char *argv[]) {
  char comando[MAX]; // Usamos un array de caracteres para almacenar el comando
<<<<<<< HEAD
  char *arg;
=======
  char *tr[MAX];
>>>>>>> definitivo
  tList commandList;
  tListF fileList;
  createList(&commandList);
  createListF(&fileList);

  while (1) {
    printf("-> ");

    fgets(comando, MAX, stdin); // Leemos la entrada del usuario

    if (comando[0] == '\0') //Si el usuario solo pulsa enter termina la funcion y vuelve al bucle
      continue;
    else {
<<<<<<< HEAD
      TrocearCadena(comando, &arg);
      insertElement(arg, &commandList);
      procesar_comando(&arg, &commandList, &fileList);
=======
      insertElement(comando, &commandList);
      TrocearCadena(comando, tr);
      procesar_comando(tr, commandList, fileList);
>>>>>>> definitivo
    }
  }

  return 0;
}
