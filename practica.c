//Javier Rodriguez Rodriguez || j.rrodriguez1@udc.es, grupo 4.2
//Miguel Corton Deben || mcortond@udc.es, grupo 4.2

#include "cabecerasAux.h"

char ** env1;

extern char ** environ;

//Funcion que realiza el comando N de la lista hist
void Cmd_comand(tList commandList, tListF fileList, tListM memoryList, tListP processList, char *tr[]) {
  int N = atoi (tr[0]);
  if (N) {
    if (countItems(commandList) <= N) {
      printf("No hay tantos comandos en hist\n");
    } else {
      char *trozos[MAX/2];
      tItemL item = getItem(atoi(tr[0]), commandList);
      TrocearCadena(item.comando, trozos);
      printf("Ejecutando hist (%d): %s\n", atoi(tr[0]), trozos[0]);
      procesar_comando(trozos, commandList, fileList, memoryList, processList);
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


//PRÁCTICA 2

//Asigna memoria con la función malloc
void Cmd_malloc(char *tr[], tListM memoryList){

  if (tr[0] == NULL) {
   	printf("******Lista de bloques asignados malloc para el proceoso %d\n", getpid());
   	printListMalloc(memoryList);
   	return;

  } else if (!strcmp(tr[0], "-free")) {

    tPosML r=NULL;
    tPosML p;
    size_t num;
    	
    if(tr[1] == NULL){  	
   	printf("******Lista de bloques asignados malloc para el proceso %d\n", getpid());

   	if(!isEmptyListM(memoryList))
     	printListMalloc(memoryList); 
     	return;
    }

    if(!isEmptyListM(memoryList)){
    	
    	if(!isNumberPos(tr[1])){	//Si el número es negativo
    	  printf("No hay bloque de ese tamaño asignado con malloc\n");
    	  return;
    	}

    	num= (size_t) atoi(tr[1]);
    	
    	for(p=(memoryList)->next; p != MNULL; p=p->next){	//encuentra el bloque con el tamaño indicado
    	  if((p->data.size) == num)
    	    r=p;
    	}
    	   	
    	if(r != NULL){
    		deleteAtPositionM(r,&memoryList);		//Libera memoria además de eliminar de la lista
    	} else printf("No hay bloque de ese tamaño asignado con malloc\n");	

    } else  printf("No hay bloque de ese tamaño asignado con malloc\n");

  } else {
    size_t tam;
    void *p;
   
    tam=(size_t) strtoul(tr[0],NULL,10);
    if(tam == 0){		//tamaño = 0
   	  printf("No se asignan bloques de 0 bytes\n");
   	  return;
    }
   
    if((p=(void *) malloc(tam*sizeof(tam))) == NULL ){		//Asigna la memoria
   	  perror("Imposible obtener memoria con malloc");
    } else {
   	  time_t t= time(NULL);
      struct tm *tm = localtime(&t);

   	  if(insertNodeM(&memoryList, p, tam, tm,"malloc",0,0,"")){		//inserta el nodo en la lista
   		  printf("Asignados %lu bytes en %p\n",(unsigned long)tam,p);
   	  } else {
   		  printf("No es posible añadirlo en la lista de bloques asignados malloc\n");
				free(p);
   	  }
    }
  } 
}

//Asigna memoria compartida
void Cmd_shared(char *tr[], tListM memoryList) {          //Probar con dos terminales diferentes. Hacer un read y en el otro terminal un memdump
  if (tr[0] == NULL) { 
    printf("******Lista de bloques asignados shared para el proceso %d\n", getpid());
    if(!isEmptyListM(memoryList)) {
   	  printListShared(memoryList);
	    return;
    }
  } else if (!strcmp(tr[0], "-create")) {   //Del fichero de ayuda
    key_t cl;
    size_t tam;
    void *p;
   
    if (tr[1]==NULL || tr[2]==NULL) {
   	  printf("******Lista de bloques asignados shared para el proceso %d\n", getpid());
      if(!isEmptyListM(memoryList)) {
   	    printListShared(memoryList);
	      return;
      }
    }

    if(!isNumberPos(tr[1])){		//la clave es negativa
		  printf ("Imposible asignar memoria compartida clave %s: %s\n", tr[1],strerror(errno));
   	  return;
    }

    cl=(key_t)  strtoul(tr[1],NULL,10);
    tam=(size_t) strtoul(tr[2],NULL,10);

    if (tam==0) {
	    printf ("No se asignan bloques de 0 bytes\n");
	    return;
    }

    if ((p=ObtenerMemoriaShmget(cl,tam, memoryList))!=NULL)
		  printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else {
		  printf ("Imposible asignar memoria compartida clave %lu: %s\n",(unsigned long) cl,strerror(errno));
		} 

  } else if (!strcmp(tr[0], "-free")) {
    int num;
    tPosML p;

    if(tr[1] == NULL){
   	  printf("******Lista de bloques asignados shared para el proceso %d\n", getpid());
   	  if(!isEmptyListM(memoryList))
   		  printListShared(memoryList);
	    return;
    }
    if(!isEmptyListM(memoryList)){
    	
      if (!isNumberPos(tr[1]) || !strcmp(tr[1],"0")){		//Clave no valida
        printf("No hay bloque de esa clave mapeado en el proceso\n");
    	  return;
      }

      num=atoi(tr[1]);
    	
      for(p=(memoryList)->next; p != MNULL; p=p->next){		//Encotrar la clave
    	  if((p->data.key) == num){ // Intenta desvincular la memoria compartida (shmdt) apuntada por el nodo.
    	    (shmdt(p->data.address) != 0) ? perror("Hubo un error con shmdt:") : removeElementM(p,&memoryList); 
    	    return;
    	    }
    	  }
        printf("No hay bloque de esa clave mapeado en el proceso\n");	  	

    } else 
      printf("No hay bloque de esa clave mapeado en el proceso\n");

  } else if (!strcmp(tr[0], "-delkey")) {

    key_t clave;
    int id;
    char *key=tr[1];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){    //Pasa la clave a un unsigned long
      printf ("      delkey necesita clave válida\n");
      return;
    }
    if ((id=shmget(clave,0,0666))==-1){       //Intenta obtener la memoria compartida 
      perror ("shmget: imposible obtener memoria compartida");
      return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1) {       //Intenta eliminar la memoria compartida 
      perror ("shmctl: imposible eliminar memoria compartida");
      return;
    }
      
  } else {
    key_t cl;
    size_t tam=0;	//Evita crear uno nuevo
    void *p;

    cl=(key_t) strtoul(tr[0],NULL,10);
		
    if ((p=ObtenerMemoriaShmget(cl,tam, memoryList))==NULL)
		  printf ("Imposible asignar memoria compartida clave %lu: %s\n",(unsigned long) cl,strerror(errno));
    }
}

//Mapea un fichero en memoria
void Cmd_mmap (char *tr[], tListM memoryList) {

  if (tr[0]==NULL){ 
    printf("******Lista de bloques asignados mmap para el proceso %d\n", getpid());
    if(!isEmptyListM(memoryList)) 
      printListMmap(memoryList); 
    return;
  } else if(!strcmp(tr[0], "-free")) {

    tPosML r=NULL;
    tPosML p;
    
    if(tr[1] == NULL){
   	  printf("******Lista de bloques asignados mmap para el proceso %d\n", getpid());
   	  if(!isEmptyListM(memoryList))
   		  printListMmap(memoryList);
	    return;
    }
    
    if(!isEmptyListM(memoryList)){
    	
    	for(p=(memoryList)->next; p != MNULL; p=p->next){	//Encuentra el fichero
    	   if(!strcmp(tr[1],p->data.nameOfFile)){
    	        r=p; 
    	   	if((munmap(p->data.address,p->data.size)) == -1)
    	   		perror("Hubo un error con munmap");
    	   	
    		  deleteAtPositionM(r,&memoryList);
    	   	return;
    	   }
    	}
  	  printf("Fichero %s no mapeado\n",tr[1]);
  		
    } else
      printf("Fichero %s no mapeado\n",tr[1]);

  } else {

    char *perm;
    void *p;
    int protection=0;

    if ((perm=tr[1])!=NULL && strlen(perm)<4) {
      if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
      if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
      if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }

    if ((p=MapearFichero(tr[0],protection, memoryList))==NULL)
      perror ("Imposible mapear fichero");
    else
      printf ("Fichero %s mapeado en %p\n", tr[0], p);
  }
}

//Lee cont bytes del fichero en una dirección de memoria
void Cmd_read(char *tr[]) {
  void *p;
  size_t cont=-1;
  ssize_t n;
  if (tr[0]==NULL || tr[1]==NULL){
	  printf ("Faltan parámetros\n");
	  return;
  }
  p=strToPointer(tr[1]);  /*convertimos de cadena a puntero*/
  if (tr[2]!=NULL)
	  cont=(size_t) atoll(tr[2]);

  if ((n=LeerFichero(tr[0],p,cont))==-1)
	  perror ("Imposible leer fichero");
  else
	  printf ("Leídos %lld bytes de %s en %p\n",(long long) n,tr[0],p);
}

//Escribe cont bytes en un fichero con una dirección de memoria
void Cmd_write(char *tr[]) {
  ssize_t n;
  size_t cont;
    
  if(tr[0] == NULL){
    printf("Faltan parámetros\n");
    return;
  }
    
  if(!strcmp(tr[0],"-o")){			
    if(tr[1] == NULL || tr[2] == NULL || tr[3] == NULL){	
      printf("Faltan parámetros\n");
      return;
    }
    cont = strtoul (tr[3],NULL,10);

    if((n=EscribirFichero(tr[1], strToPointer(tr[2]), cont,1)) == -1)	//Último parámetro a 1 para sobreescribir
      perror("Hubo un error escribiendo el fichero");
    else
     printf ("Escritos %s bytes en %s desde %p\n",tr[3],tr[1],strToPointer(tr[2]));
  } else {
    if(tr[0] == NULL || tr[1] == NULL || tr[2] == NULL){	
      printf("Faltan parámetros\n");
    	return;
    }
    cont = strtoul (tr[2],NULL,10);		//Devuelve un valor de tipo unsigned long (no hay valores negativos)
    if((n=EscribirFichero(tr[0], strToPointer(tr[1]), cont,0)) == -1)
      perror("Hubo un error escribiendo el fichero");
    else
      printf ("Escritos %s bytes en %s desde %p\n",tr[2],tr[0],strToPointer(tr[1]));
  }
}

void Cmd_memdump(char *tr[]){

    size_t c = 25;		//deafult line=25
    
    if(tr[0]==NULL) return;
    
    if(tr[1] != NULL) c=(size_t) strtoul(tr[1],NULL,10);
    
    printf("Volcando %zu bytes desde la dirección %p\n",c,strToPointer(tr[0]));
    showMemory(strToPointer(tr[0]),c);
}

//Llena la memoria a partir de addr con byte
void Cmd_memfill(char *tr[]) {
  void *p; 
  int cont;
  unsigned char byte;

  if(tr[0] != NULL) {     //Comprueba si hay una dirección de memoria
    p = cadtop(tr[0]);    //Convierte esa dirección en un puntero
    if (tr[1] == NULL)
      cont = 128;     //Si no se le pasa el argumento cont (número de bytes) utiliza 128 por defecto
    if(tr[2]==NULL)
      byte = (unsigned char)'A';    //Si no se le pasa el valor del byte utiliza A por defecto
    else {
      cont = (size_t) strtoul(tr[1],NULL,10);	;     //Convierte la cadena con el número de bytes un valor entero
      byte = atoi(tr[2]);                           //Convierte la cadena con el valor del byte a un valor entero
    }
    printf("Llenando %d bytes de memoria con %c(%02x) en %p\n", cont, byte, byte, p);     //%02x porque es hexadecimal
    LlenarMemoria(p, cont, byte); 
  } else 
    return;
}

void mem_funcs() {
  printf("Funciones programa\t %p, %p, %p\n", mem_funcs, Cmd_authors, Cmd_date);    //%p imprime las direcciones de memoria en hexadecimal
  printf("Funciones librería\t %p, %p, %p\n", strcmp, sscanf, printf);
}

int gl1=1,gl2=2,gl3=3;
void mem_vars(){
  static int st1=4,st2=5,st3=6;
  int loc1=10,loc2=11,loc3=12;
  printf("Variables locales\t %p, %p, %p\n", &loc1, &loc2, &loc3);
  printf("Variables globales\t %p, %p, %p\n", &gl1, &gl2, &gl3);
  printf("Variables estáticas\t %p, %p, %p\n", &st1, &st2, &st3);
}

//Muestra detalles de la memoria del proceso
void Cmd_mem(char *tr[], tListM memoryList) {
  if((tr[0]==NULL) || (strcmp(tr[0],"-all")==0)) {
    mem_vars();
    mem_funcs();
    printListM(memoryList);
  } else if(strcmp(tr[0], "-blocks")==0) {
    printListM(memoryList);
  } else if(strcmp(tr[0], "-funcs")==0) {
    mem_funcs();
  } else if(strcmp(tr[0], "-vars")==0) {
    mem_vars();
  } else if(strcmp(tr[0], "-pmap")==0) {
    mem_pmap();
  } else {
    printf("Opcion %s no contemplada\n", tr[0]);
  }
}

//Invoca a la funcion recursiva n veces
void Cmd_recursiva(char *tr[]) {        //Fijarse porque salen esas direcciones, lo va a preguntar en la defensa
  if(tr[0]!=NULL){
    long n = strtol(tr[0], NULL, 10);
    int num=(int)n;
    Recursiva(num);
  }
}


//PRÁCTICA 3

void Cmd_uid(char *tr[]) {
  if ((tr[0] == NULL) || (!strcmp(tr[0], "-get"))) {
    mostrarCredenciales();
    return;
  } else if (!strcmp(tr[0], "-set")) {
    if (tr[1] == NULL) {
      mostrarCredenciales();
      return;
    }
    
    uid_t new_uid;
    
    if (!strcmp(tr[1], "-l")) {
      if (tr[2] == NULL) {
        mostrarCredenciales();
      } else {
        new_uid = atoi(tr[2]);
        if (setuid(new_uid) == -1) {
          perror("Imposible cambiar credencial");
          return;
        }
      }
    } else {
      new_uid = atoi(tr[1]);
      if (seteuid(new_uid) == -1) {
        perror("Imposible cambiar credencial");
        return;
      }
    }
  }
}

void Cmd_showenv(char *tr[]){
  if(tr[0] == NULL){
    showEnvironment(env1,"main arg3");
    return;
  }
    
  if(!strcmp(tr[0],"-environ"))
    showEnvironment(environ,"environ");
  else if(!strcmp(tr[0],"-addr")){
    printf("environ:   %p (almacenado en %p)\n",&environ[0],&environ);
    printf("main arg3: %p (almacenado en %p)\n",&environ[0],&env1);
  }else
    printf("Uso: showenv [-environ|-addr]\n");
}

void Cmd_showvar(char *tr[]) {
  int pos;
  char *s[1];
  s[0]=NULL;
    
  if(tr[0] == NULL){
    Cmd_showenv(s);
    return;
  }
    
  if( (pos = BuscarVariable(tr[0],environ)) == -1)
    return;
  
  //printf("Con arg3 main %s(%p) @%p\n",environ[pos],environ[pos],env1);
  printf("Con environ %s(%p) @%p\n",environ[pos],environ[pos],&environ[pos]);
  printf("Con getenv %s(%p)\n",getenv(tr[0]),&environ[pos]);
}

void Cmd_changevar(char *tr[]){
    
    char new[MAX]="";
    
    if(tr[0] == NULL || tr[1] == NULL || tr[2] == NULL){
    	printf("Uso: changevar [-a|-e|-p] var valor\n");
    	return;
    }
    
    if(!strcmp(tr[0],"-a")){
      if (tr[1] == NULL || tr[2] == NULL) {
        printf("Uso: changevar [-a|-e|-p] var valor\n");
    	  return; 
      }
    	if((CambiarVariable(tr[1],tr[2],env1)) == -1){
    	    perror("Imposible cambiar variable"); //Aqui creo que mejor poner perror
    	    return;
    	}
    }
    else if(!strcmp(tr[0],"-e")){
      if (tr[1] == NULL || tr[2] == NULL) {
        printf("Uso: changevar [-a|-e|-p] var valor\n");
    	  return; 
      }
    	if((CambiarVariable(tr[1],tr[2],environ)) == -1){
    	    perror("Imposible cambiar variable");
    	    return;
    	}
    }
    else if(!strcmp(tr[0],"-p")){
      if (tr[1] == NULL || tr[2] == NULL) {
        printf("Uso: changevar [-a|-e|-p] var valor\n");
    	  return; 
      }
    	strcpy(new,tr[1]);
    	strcat(new,"=");
    	strcat(new,tr[2]);
    	putenv(new);
    }
    else  printf("Uso: changevar [-a|-e|-p] var valor\n");
}

void Cmd_subsvar(char *tr[]) {
  if (tr[0] == NULL || tr[1] == NULL || tr[2] == NULL || tr[3] == NULL) {
    printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
    return;
  } 
  if (!strcmp(tr[0], "-a")) {
    char message[MAX];
    char *e[] = {NULL};
    int pos = CambiarVariable(tr[1], tr[2], e);
    if (pos == -1) {
      snprintf(message, sizeof(message), "Imposible sustituir variable %s por %s", tr[1], tr[2]);
      perror(message);
      return;
    }
    printf("Variable de entorno actualizada: %s=%s\n", tr[2], tr[3]);
  } else if (!strcmp(tr[0], "-e")) {
    char message[MAX];
    int pos = BuscarVariable(tr[1], environ);
    if (pos == -1) {
      snprintf(message, sizeof(message), "Imposible sustituir variable %s por %s", tr[1], tr[2]);
      perror(message);
      return;
    }

    environ[pos] = malloc(strlen(tr[2]) + strlen(tr[3]) + 2);
    if (environ[pos] == NULL) {
      perror("Error al asignar memoria");
      return;
    }

    strcpy(environ[pos], tr[2]);
    strcat(environ[pos], "=");
    strcat(environ[pos], tr[3]);

    printf("Variable de entorno actualizada: %s=%s\n", tr[2], tr[3]);
  }
}

void Cmd_fork(tListP *P){
    forkaux();
    deleteListP(P);
}

void Cmd_exec(char* tr[]){}

void Cmd_jobs(tListP *P){
    printListP(*P);
}

void Cmd_deljobs(char *tr[], tListP *P){
    if(tr[0]==NULL){
        printListP(*P);
    }else if(strcmp(tr[0], "-term")==0){
        removeTermP(P);
        printListP(*P);
    }else if(strcmp(tr[0], "-sig")==0){
        removeSigP(P);
        printListP(*P);
    }else printListP(*P);
}

void Cmd_job(char *tr[], tListP *P) {
  pid_t pid;
  int st;
  tPosPL p;
  if(tr[0]==NULL)
      printListP(*P);
  else{
      if(strcmp(tr[0], "-fg")==0){
          pid=atoi(tr[1]);
          if((p = searchPid(pid, *P))!=NULL){
              waitpid(pid,&st,0);
              printf("Proceso %d terminado normalmente. Valor devuelto %d\n", pid, st);
              removeElemP(p, P);
          }
      }else
          printJob(atoi(tr[0]), *P);
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
  else if (!strcmp(tr[0], "malloc")) {
    printf("malloc [-free] [tam]: Asigna un bloque de memoria de tamano tam con malloc\n");
    printf("\t-free: deasigna un bloque de memoria de tamano tam asignado con malloc\n");
  }
  else if (!strcmp(tr[0], "shared")) {
    printf("shared [-free|-create|-delkey] cl [tam]: Asigna memoria compartida con la clave cl en el programa\n");
    printf("\t-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamaño tam\n");
    printf("\t-free cl: desmapea el bloque de memoria compartida de clave cl\n");
    printf("\t-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl\n");
  }
  else if (!strcmp(tr[0], "mmap")) {
    printf("mmap [-free] fich prm: Mapea el fichero fich con permisos prm\n");
    printf("\t-free fich: desmapea el fichero fich\n");
  }
  else if (!strcmp(tr[0], "read")) {
    printf("read fiche addr cont: Lee cont bytes desde fich a la dirección addr\n");
  }
  else if (!strcmp(tr[0], "write")) {
    printf("write [-o] fiche addr cont: Escribe cont bytes desde la dirección addr a fich (-o sobreescribe)\n");
  }
  else if (!strcmp(tr[0], "memdump")) {
    printf("memdump addr cont: Vuelca en pantallas los contenidos (cont bytes) de la posición de memoria addr\n");
  }
  else if (!strcmp(tr[0], "memfill")) {
    printf("memfill addr cont byte: Llena la memoria a partir de addr con byte\n");
  }
  else if (!strcmp(tr[0], "mem")) {
    printf("mem [-block|-funcs|-vars|-all|-pmap] ..: Muestra detalles de la memoria del proceso\n");
    printf("\t-blocks: los bloques de memoria asignados\n");
    printf("\t-funcs: las direcciones de las funciones\n");
    printf("\t-vars: las direcciones de las variables\n");
    printf("\t-all: todo\n");
    printf("\t-pmap: muestra las salida del comando pmap (o similar)\n");
  }
  else if (!strcmp(tr[0], "recursiva")) {
    printf("recursiva [n]: Invoca a la función recursiva n veces\n");
  }
  else if (!strcmp(tr[0], "uid")) {
    printf("uid [-get|-set] [-l] [id]: Accede a las credenciales del proceso que ejecuta el shell\n");
    printf("\t-get: muestra las credenciales\n");
    printf("\t-set id: establece la credencial al valor numerico id\n");
    printf("\t-set -l id: establece la credencial a login id\n");
  }
  else if (!strcmp(tr[0], "showvar")) {
    printf("showvar var: Muestra el valor y las direcciones de la variable de entorno var\n");
  }
  else if (!strcmp(tr[0], "changevar")) {
    printf("changevar [-a|-e|-p] var valor: Cambia el valor de una variable de entorno\n");
    printf("\t-a: accede por el tercer arg de main\n");
    printf("\t-e: accede mediante environ\n");
    printf("\t-p: accede mediante putenv\n");
  }
  else if (!strcmp(tr[0], "subsvar")) {
    printf("subsvar [-a|-e] var1 var2 valor: Sustituye la variable de entorno var1 con var2 = valor");
    printf("\t-a: accede por el tercer argumento de arg de main\n");
    printf("\t-e: accede mediante environ\n");
  }
  else if (!strcmp(tr[0], "showenv")) {
    printf("showenv [-environ|-addr]: Muestra el entorno del proceso\n");
    printf("\t-environ: accede usando environ (en lugar del tercer arg de main)\n");
    printf("\t-addr: muestra el valor y donde se almacenan environ y el tercer argumento de arg de main\n");
  }
  else if (!strcmp(tr[0], "fork")) {
    printf("fork: El shell hace fork y queda en espera a que su hijo temrine\n");
  }
  else if (!strcmp(tr[0], "exec")) {
    printf("exec VAR1 VAR2 ..prog args....[@pri]: Ejecuta, sin crear un proceso, prog con argumentos en un entorno que contiene solo variables VAR1, VAR2...\n");
  }
  else if (!strcmp(tr[0], "jobs")) {
    printf("jobs: Lista los procesos en segundo plano\n");
  }
  else if (!strcmp(tr[0], "deljobs")) {
    printf("deljobs [-term] [-sig]: Elimina todos los procesos de la lista procesos en sp\n");
    printf("\t-term: los terminados\n");
    printf("\t-sig: los terminados por senal\n");
  }
  else if (!strcmp(tr[0], "job")) {
    printf("job [-fg] pid: Muestra información del proceso pid\n");
    printf("\t-fg: lo pasa a primer plano\n");
  }
  else
    printf("%s no encontrado\n", tr[0]);
}

void Cmd_exit(tListF fileList, tList commandList, tListM memoryList, tListP processList){	//Libera la memoria y luego finaliza el programa
  freeList(&commandList);
  freeListF(&fileList);
  freeListM(&memoryList);
  freeListP(&processList);
  free(commandList);
  free(fileList);
  free(memoryList);
  free(processList);
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
  {"read", Cmd_read},
  {"write", Cmd_write},
  {"memdump", Cmd_memdump},
  {"memfill", Cmd_memfill},
  {"recursiva", Cmd_recursiva},
  {"uid", Cmd_uid},
  {"showvar", Cmd_showvar},
  {"showenv", Cmd_showenv},
  {"changevar", Cmd_changevar},
  {"subsvar", Cmd_subsvar},
};

//Funciones para el comando externo, para probar si funcionan el job y deljobs → las funciones siguientes funcionan pero hay que colocarlas bien en el código
/*
bool insertElementP(int pid, char* comm, tListP *P){
    tPosPL q,r; struct passwd *p;
    char fecha[MAX];
    char* formato = "%y/%m/%d %H:%M:%S";
    uid_t user;
    //se crea un nodo (si es posible)
    if (!createNodeP(&q))    return false;
    else{
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        strftime(fecha, MAX, formato, local);
        q->data.time = strdup(fecha);
        q->data.pid = pid;
        user = getuid();
        p = getpwuid(user);
        q->data.uid = strdup(p->pw_name);
        q->data.priority = getpriority(PRIO_PROCESS, q->data.pid);
        q->data.command = strdup(comm);
        strcpy(q->data.status, "ACTIVO");
        q->data.sign = 0;
        q->next = NULL;
        if(*P==NULL) *P=q;
        else {
            for (r = *P; r->next != NULL; r = r->next); //move to end
            r->next=q;
        }return true;
    }
}

int getPrioExt(char* pri){
    int j;
    char auxpri[MAX];
    for(j=0; pri[j+1]; j++){
        auxpri[j]=pri[j+1];
    }
    return atoi(auxpri);
}

void identifyData(char* argv[], char* var[], char* prog[], int *prio, int *bg){
    int i, j, k=0, a, terminar=0, variable;
    
    for(i=0; argv[i]!=NULL && terminar==0; i++){ //buscamos las variables en las que se ejecutará prog
        if((variable=BuscarVariable(argv[i], environ))!=-1){
            var[i]=environ[variable];
        }else terminar=1;
    }
    if(i==0) a=0; else a=i-1;
    if(argv[a]!=NULL){ 
        for(j=a; argv[j]!=NULL && strcmp(argv[j], "&")!=0 ; j++){ //buscamos prog args
            prog[k]=argv[j];
            k++;
        }
        if(argv[j]==NULL){
            *bg = 0;
        }else if(strcmp(argv[j], "&")==0){
            *bg=1;
        }
        if(prog[0]!=NULL){
            char* cprio = prog[k-1];
            if(cprio[0]=='@'){
                *prio = getPrioExt(cprio);
                for(j=0; prog[j+1]!=NULL; j++);
                prog[j]=NULL;
            }
        }
    }else{
        printf("Argumentos insuficientes\n");
    }   
}

const char * Ejecutable (const char *s)
{
	char path[MAX];
	static char aux2[MAX];
	struct stat st;
	char *p;
	if (s==NULL || (p=getenv("PATH"))==NULL)
		return s;
	if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;       /*is an absolute pathname
	strncpy (path, p, MAX);
	for (p=strtok(path,":"); p!=NULL; p=strtok(NULL,":")){
       sprintf (aux2,"%s/%s",p,s);
	   if (lstat(aux2,&st)!=-1)
		return aux2;
	}
	return s;
}

int OurExecvpe(const char *file, char *const argv[], char *const envp[])
{
   return (execve(Ejecutable(file),argv, envp));
}

void exterprog(char* argv[], tListP *P){
    int prio, bg, pid, i, st;
    char* var[MAX]={}; char *prog[MAX]={};   
    char aux[MAX] = "";
    prio=0; bg=0;
    
    identifyData(argv, var, prog, &prio, &bg);  
    
    if(prog[0]!=NULL){
        if((pid=fork())==-1){ 
            perror("No se puede hacer fork");
        }else if(pid>0){ //soy el padre (pid hijo)
            if(prio!=0) setpriority(PRIO_PROCESS, pid, prio);
            if(bg==1){
                for(i=0; argv[i]!=NULL; i++){
                    strcat(aux, argv[i]);
                    strcat(aux, " ");
                }
                insertElementP(pid, aux, P);
            } 
            else waitpid(pid, &st, 0);
        }else if(pid==0){ //soy el hijo 
            if(var[0]!=NULL){ 
                OurExecvpe(prog[0], prog, var);
            }else{
                OurExecvpe(prog[0], prog, environ);
            }
        }
    }else printf("Faltan parámetros\n");
    

}*/

void procesar_comando(char *tr[], tList comandList, tListF fileList, tListM memoryList, tListP *processList) {

  if (tr[0] == NULL)
    return;
  if (!strcmp("comand", tr[0]))
    Cmd_comand(comandList, fileList, memoryList, processList, tr+1);
  else if (!strcmp("hist", tr[0]))
    Cmd_hist(&comandList, tr+1);
  else if (!strcmp("exit", tr[0]) || !strcmp("quit", tr[0]) || !strcmp("bye", tr[0]))
    Cmd_exit(fileList, comandList, memoryList, processList);
  else if (!strcmp("open", tr[0]))
    Cmd_open(tr+1, fileList);
  else if (!strcmp("close", tr[0]))
    Cmd_close(tr+1, fileList);
  else if (!strcmp("dup", tr[0]))
    Cmd_dup(tr+1, fileList);
  else if (!strcmp("listopen", tr[0]))
    Cmd_listopen(tr+1, fileList);
  else if (!strcmp("malloc", tr[0]))
    Cmd_malloc(tr+1, memoryList);
  else if (!strcmp("shared", tr[0]))
    Cmd_shared(tr+1, memoryList);
  else if (!strcmp("mmap", tr[0]))
    Cmd_mmap(tr+1, memoryList);
  else if (!strcmp("mem", tr[0]))
    Cmd_mem(tr+1, memoryList);
  else if (!strcmp("fork", tr[0]))
    Cmd_fork(processList);
  else if (!strcmp("jobs", tr[0]))
    Cmd_jobs(processList);
  else if (!strcmp("deljobs", tr[0]))
    Cmd_deljobs(tr+1, processList);
  else if (!strcmp("job", tr[0]))
    Cmd_job(tr+1, processList);
  else {
    exterprog(tr,processList);
  else {
    int i;
    pid_t pid;
    int k=0;    
    char aux[MAX]=""; 
    char state[MAX]="ACTIVE";
    char thisTime[MAX];
    for (i=0; cmds[i].nombre != NULL; i++){
      if (!strcmp(cmds[i].nombre, tr[0])) {
        (cmds[i].pfuncion) (tr+1);
        return;
      }
    }
    if(tr[0] == NULL){
      Cmd_exec(tr);
      return;
    }
        
    for(k=0; tr[k] != NULL; k++);       
        
    if((pid = fork()) == 0){
      Cmd_exec(tr);
      Cmd_exit(fileList, comandList, memoryList, processList);
            
      }else{
        if(!strcmp(tr[k-1],"&")){                  
               
          if((k-1) == 0)
            strcpy(aux,"NULL");
          else{
            for(int j=0; tr[j] != NULL; j++)
              if(strcmp(tr[j],"&")){
                strcat(aux,tr[j]); 
                strcat(aux," ");                  
              }
          }  	   
               	                
          insertNodeP(&processList, pid, getuid(), getTime(thisTime), state, 0, aux, 0); 
               
        }else{
          waitpid(pid,NULL,0);
        }
      }*/
    printf("Comando %s no encontrado. Consulte la lista de comandos disponibles con help\n", tr[0]);
  }
}

int main(int argc, char *argv[], char *env[]) {
  char comando[MAX]; // Usamos un array de caracteres para almacenar el comando
  char *tr[MAX];
  tList commandList;
  tListF fileList;
  tListM memoryList;
  tListP processList;
  createList(&commandList);
  createListF(&fileList);
  createListM(&memoryList);
  createListP(&processList);
  env1 = env;

  while (1) {
    printf("-> ");

    fgets(comando, MAX, stdin); // Leemos la entrada del usuario

    comando[strcspn(comando, "\n")] = '\0'; // Eliminamos el carácter de salto de línea

    if (comando[0] == '\0') //Si el usuario solo pulsa enter termina la funcion y vuelve al bucle
      continue;
    else {
      insertElement(comando, &commandList);
      TrocearCadena(comando, tr);
      procesar_comando(tr, commandList, fileList, memoryList, processList);
    }
  }
  return 0;
}
