//Javier Rodriguez Rodriguez || j.rrodriguez1@udc.es, grupo 4.2
//Miguel Corton Deben || mcortond@udc.es, grupo 4.2

#include "comand_list.h"
#include "cabecerasAux.h"

//Funcion que realiza el comando N de la lista hist
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
void Cmd_hist(tList *commandList, char *tr[]) {
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
  int df;
    
  if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
    printListF(fileList);
    return;
  }

  if (close(df)==-1)
    perror("Imposible cerrar descriptor");
  else {
    removeElementF(df, &fileList);
  }
}

void Cmd_open (char *tr[], tListF fileList) {
  int i,df, mode=0;
    
  if (tr[0]==NULL) { /*no hay parametro*/
    printListF(fileList);
    return;
  }

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
    tItemF newItem;
    newItem.descriptor = df;
    newItem.mode = mode;
    strncpy(newItem.nombre, tr[0], MAX);
    insertElementF(newItem, &fileList);
    printf("Añadida entrada a la tabla ficheros abiertos: descriptor %d, modo %d, nombre %s\n", df, mode, tr[0]);
  }
}

void Cmd_dup (char * tr[], tListF fileList) { 

  int df;
  char aux[MAX], *p;
  
  if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
    printListF(fileList);           /*o el descriptor es menor que 0*/
    return;
  }
  if (existsDescriptor(df, fileList)) {
    p = getItemF(df, fileList).nombre;
    tItemF item;
    sprintf(aux, "dup %d (%s)", df, p);
    item.descriptor = fcntl(df,F_DUPFD);
    strncpy(item.nombre, aux, MAX);
    insertElementF(item, &fileList);
  } else
    printf("Imposible duplicar descriptor: Bad file descriptor\n");
}

void Cmd_listopen(char *tr[], tListF fileList) {
  if (tr[0] == NULL)
    printListF(fileList);
  else if (atoi(tr[0])) {
    int ncmd;
    ncmd=(int) (abs(strtol(tr[0],NULL,10)));
    printUntilNF(fileList, ncmd);
  } else 
    printf("Opcion no encontrada\n");
}

//Imprime el pid del comando que se esta ejecutando el la red 
void Cmd_pid(char *tr[]) {
  if (tr[0] == NULL)
    printf("Pid del shell: %d \n", getpid());
  else if (strcmp(tr[0], "-p") == 0)
    printf("Pid del padre del shell: %d \n", getppid());
  else
    printf("Opcion no encontrada\n");
}

//Funcion para obtener la fecha del sistema
void Cmd_date(char *tr[]) {
  if (tr[0] == NULL) {
    // Obtener la fecha actual
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    // Formatear la fecha en el formato DD/MM/YYYY
    char fecha[11]; // Espacio suficiente para almacenar "DD/MM/YYYY\0"
    strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm_info);

    // Imprimir la fecha en el formato correcto
    printf("%s\n", fecha);
  } else
    printf("Opcion no encontrada\n");
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

    // Imprimir la hora formateada
    printf("%s\n", hora);
  } else
    printf("Opcion no encontrada\n");
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
  else if(chdir(tr[0]) == -1) {
    perror("Imposible cambiar directorio");
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
  } else
    printf("Opcion no encontrada\n");
}

void Cmd_create(char *tr[]){
    
  char dir[MAX];
    
  if(tr[0]==NULL){
    printf("%s\n",getcwd(dir,MAX));
  }else if(!strcmp(tr[0],"-f")){
    if(tr[1] == NULL) { 
      printf("%s\n",getcwd(dir,MAX)); 
    }else{        
      int ch= open(tr[1], O_CREAT | O_EXCL, 0775); //CREAT, crea nuevos ficheros, EXCL evita sobreescribir
    	if(ch == -1) perror("Imposible crear");
    }
  }else{
    if(mkdir(tr[0],0775) == -1) perror("Imposible crear");
  }
}

void Cmd_stat(char *tr[]){

  char dir[MAX];
    
  if(tr[0] == NULL){
    printf("%s\n",getcwd(dir,MAX));
  }else{    
    struct statParams pr={0,0,0,0,0,0};
    int counterFiles=0;      
    pr=getParams(tr,pr);
      
    for(int i=0; tr[i]!=NULL ; i++){
      if((strcmp(tr[i],"-long")) && (strcmp(tr[i],"-acc")) && (strcmp(tr[i],"-link"))){
        printStats(tr[i],&pr);
        counterFiles++;
      }           	
    }
    if(counterFiles==0){ //if we type the command with parameters but there isn't any file
      printf("%s\n",getcwd(dir,MAX));
    } 
  }    
}


void Cmd_delete(char *tr[]){
  char dir[MAX];
  int i=0;
  char message[MAX];
  if(tr[0]==NULL) {
    printf("%s \n", getcwd(dir,MAX));
  } else {
    while(tr[i]!=NULL){
      strcat(strcpy(message,"Imposible borrar "), tr[i]);  //Al colocar el mensaje aquí sí funciona, si no da Violación de segmento
      if(remove(tr[i])==-1)
        perror(message);
      i++;
    }
  }
}

/*
OPCIONES:
- directorio vacío → no se puede borrar
- directorio con otro directorio dentro → entra y vuelve al inicio (recursividad)
- directorio con ficheros dentro → borra el directorio
- ficheros → borra los ficheros
*/
void Cmd_deltree(char *tr[]) { 
  char dir[MAX];
  int i=0;
  int j;
  struct stat check;
  DIR *direct;
  struct dirent *direntd;
  char *aux[MAX];
  char message[MAX];

  if(tr[0]==NULL)
    printf("%s \n", getcwd(dir,MAX));
  else {
    while(tr[i]!=NULL) {
      j=0;
      char *string = strdup(tr[i]);
      if(lstat(string, &check)!=0) {
        strcat(strcpy(message,"Imposible borrar "), tr[i]);
        perror(message);
      } else {
        if(LetraTF(check.st_mode) != 'd') { //si no es un directorio
          printf("Borrando fichero %s\n", tr[i]);
          if(remove(tr[i])==-1) { 
            strcat(strcpy(message,"Imposible borrar "), tr[i]);
            perror(message);
          } return;
        } else {
          if(remove(tr[i])==-1) {
            if((direct=opendir(tr[i]))==NULL){
              strcat(strcpy(message,"Imposible abrir directorio "), tr[i]);
              perror(message);
            }
            else {
              while((direntd = readdir(direct)) != NULL) {
                if((strcmp(direntd->d_name,".")!=0) && (strcmp(direntd->d_name,"..")!=0)) {
                  aux[j]=direntd->d_name;
                  j++;
                }
              } closedir(direct);
              if(j==0)
                remove(tr[i]);
              else {
                chdir(string);
                Cmd_deltree(aux);
                chdir("..");
              }
              if(remove(string)==-1) {
                strcat(strcpy(message,"Imposible borrar "), tr[i]);
                perror(message);
              }
            }
          }
        }
      } i++;
    }
  }
}

void Cmd_list(char *tr[]){
  char dir[MAX];
    
  if(tr[0] == NULL){
    printf("%s\n",getcwd(dir,MAX));
  }else{
    struct statParams pr={0,0,0,0,0,0};
    int counterFiles=0;   	
    pr=getParams(tr,pr);

    for(int i=0; tr[i]!=NULL ; i++){
      
      if((strcmp(tr[i],"-long")) && (strcmp(tr[i],"-acc")) && (strcmp(tr[i],"-link")) && (strcmp(tr[i],"-hid")) && (strcmp(tr[i],"-reca")) && (strcmp(tr[i],"-recb")) ){
        printLstats(tr[i],&pr);
      	counterFiles++;
      }           	
    }        
    if(counterFiles==0){ //if we type the command with parameters but there isn't any file
      printf("%s\n",getcwd(dir,MAX));
    } 
  }    
}

//Imprime información sobre el comando que se le pasa, si no pasa comando muestra por pantalla los comandos disponibles
void Cmd_help(char *tr[]) {
  if(tr[0] == NULL){
    printf("Ayuda sobre los comandos, comandos disponilbes:\n-authors\n-pid\n-date\n-time\n-hist\n-comand\n-infosys\n-help\n-exit\n-quit\n-bye\n-open\n-close\n-dup\n-listopen\n-create\n-stat\n-list\n-delete\n-deltree\n");
  }
  else if (!strcmp(tr[0],"authors")){
    printf("authors [-l][-n]: Muestra los nombres y/o logins de los autores\n");
  }
  else if (!strcmp(tr[0],"pid")){
    printf("pid [-p]: Muestra el pid del shell o de su proceso padre\n");
  }
  else if (!strcmp(tr[0], "chdir")) {
    printf("chdir [dir]: Cambia (o muestra) el directorio actual del shell\n");
  }
  else if (!strcmp(tr[0],"date")){
    printf("date: Muestra la fecha actual\n");
  }
  else if (!strcmp(tr[0], "time")) {
    printf("time: Muestra la hora actual\n");
  }
  else if (!strcmp(tr[0],"hist")){
    printf("hist [-c][-N]: Muestra (o borra) el historico de comandos\n");
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
    printf("listopen [n]: Lista los ficheros abiertos (al menos n) del shell\n");
  }
    else if (!strcmp(tr[0], "create")){
    printf("create [-f] [name]: Crea un directorio o un fichero (-f)\n");
  }
  else if (!strcmp(tr[0], "stat")){
    printf("stat [-long] [-link] [-acc] name1 name2 ... lista ficheros\n");
    printf("\t-long: listado largo\n");
    printf("\t-acc: accesstime\n");
    printf("\t-link: si es enlace simbolico, el path contenido\n");
  }
  else if (!strcmp(tr[0], "list")){
    printf("list [-reca] [-recb] [-hid] [-long] [-link] [-acc] n1 n2 ... lista de directorios\n");
    printf("\t-hid: incluye los ficheros ocultos\n");
    printf("\t-recb: recursivo(antes)\n");
    printf("\t-reca: recursivo(despues)\n");
    printf("\tresto: parametros como stat\n");
  }
  else if (!strcmp(tr[0], "delete")){
    printf("delete [name1 name2 ...]: Borra ficheros o directorios vacios\n");
  }
  else if (!strcmp(tr[0], "deltree")){
    printf("deltree [name1 name2 ...]: Borra ficheros o directorios no vacios recursivamente\n");
  }
  else
    printf("%s no encontrado\n", tr[0]);
}

void Cmd_exit(tListF fileList, tList commandList){
  freeList(&commandList);
  freeListF(&fileList);
  free(commandList);
  free(fileList);
  exit(0);
}

struct cmd cmds[]={
  {"date",Cmd_date},
  {"time",Cmd_time},
  {"infosys",Cmd_infosys},
  {"authors",Cmd_authors},
  {"pid",Cmd_pid},
  {"chdir",Cmd_chdir},
  {"help",Cmd_help},
  {"create", Cmd_create},
  {"delete", Cmd_delete},
  {"stat", Cmd_stat},
  {"deltree", Cmd_deltree},
  {"list", Cmd_list},
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
  else if (!strcmp("open", tr[0]))
    Cmd_open(tr+1, fileList);
  else if (!strcmp("close", tr[0]))
    Cmd_close(tr+1, fileList);
  else if (!strcmp("dup", tr[0]))
    Cmd_dup(tr+1, fileList);
  else if (!strcmp("listopen", tr[0]))
    Cmd_listopen(tr+1, fileList);
  else {
    for (i=0; cmds[i].nombre != NULL; i++){
      if (!strcmp(cmds[i].nombre, tr[0])) {
        (cmds[i].pfuncion) (tr+1);
        return;
      }
    }
    printf("Comando %s no encontrado. Consulte la lista de comandos disponibles con help\n", tr[0]);
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