//Javier Rodriguez Rodriguez, grupo 4.2
//Miguel Corton Deben, grupo 4.2

#include "cabecerasAux.h"

int TrocearCadena(char *cadena, char *trozos[]){
  int i=1;
  if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    return 0;
  while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
  return i;
}

//Set 1 to each parameter if it is input
struct statParams getParams(char *tr[], struct statParams pr){

  for(int i=0; tr[i] != NULL; i++){
    if(!strcmp(tr[i],"-long")) 	 pr.lon=1;
    else if (!strcmp(tr[i],"-acc"))  pr.acc=1;
    else if (!strcmp(tr[i],"-link")) pr.lnk=1;
    else if (!strcmp(tr[i],"-hid"))  pr.hid=1;
    else if (!strcmp(tr[i],"-reca")) pr.reca=1;
    else if (!strcmp(tr[i],"-recb")) pr.recb=1;
  }
  return pr;
}

char *lastDir(char *directories){
  char *dirs[MAX/2];
  int i=1;
    
  dirs[0]=strtok(directories, "/ ");
  while((dirs[i]=strtok(NULL, "/ "))!=NULL)
    i++;
  if(i==1) return dirs[0]; //only one token
  else return dirs[i-1];   //otherwise, last one
}

char LetraTF (mode_t m) {
  switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
    case S_IFSOCK: return 's'; /*socket */
    case S_IFLNK: return 'l'; /*symbolic link*/
    case S_IFREG: return '-'; /* fichero normal*/
    case S_IFBLK: return 'b'; /*block device*/
    case S_IFDIR: return 'd'; /*directorio */ 
    case S_IFCHR: return 'c'; /*char device*/
    case S_IFIFO: return 'p'; /*pipe*/
    default: return '?'; /*desconocido, no deberia aparecer*/
  }
}


char * ConvierteModo2 (mode_t m) {
  static char permisos[12];
  strcpy (permisos,"---------- ");
    
  permisos[0]=LetraTF(m);
  if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
  if (m&S_IWUSR) permisos[2]='w';
  if (m&S_IXUSR) permisos[3]='x';
  if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
  if (m&S_IWGRP) permisos[5]='w';
  if (m&S_IXGRP) permisos[6]='x';
  if (m&S_IROTH) permisos[7]='r';    /*resto*/
  if (m&S_IWOTH) permisos[8]='w';
  if (m&S_IXOTH) permisos[9]='x';
  if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
  if (m&S_ISGID) permisos[6]='s';
  if (m&S_ISVTX) permisos[9]='t';
    
  return permisos;
}  

void printStats(char *tr, struct statParams *pr){
  struct stat fs; 
  // lstad instead of stat because if path is a symbolic link,then the link itslef is stat-ed, not the file    
  char copy[MAX];
  strcpy(copy,tr);
    
  if(lstat(tr,&fs)){     
    fprintf(stderr, "****error al acceder a %s: ", tr);
    perror("");
    return;
  }
    
  int total= pr->lon + pr->lnk + pr->acc + pr->hid + pr->reca + pr->recb;
  
  if(total==0){    	
    printf("% 8ld  %s\n", fs.st_size,lastDir(tr));
  }else{
       	
    //-long option
    if (pr->lon){
    	
    struct group *gr;
    struct passwd *pw;
    char buffer[MAX];
    char symBuff[MAX]="";  //initialize to "" for avoiding memory errors
    
    if((pw=getpwuid(fs.st_uid)) == NULL) {
      fprintf(stderr, "****error al acceder a %s ", tr);
      perror("");
    }
    if((gr=getgrgid(fs.st_gid)) == NULL) {
      fprintf(stderr, "****error al acceder a %s ", tr);
      perror("");
    }
    	
    //-acc option
    if((pr->acc) == 1) strftime(buffer,MAX,"%Y/%m/%d-%H:%M",localtime(&fs.st_atime));
    else strftime(buffer,MAX,"%Y/%m/%d-%H:%M",localtime(&fs.st_mtime));
    	 	
    printf("%s   %ld (  %ld)   %s   %s",buffer,fs.st_nlink,fs.st_ino,pw->pw_name,gr->gr_name);
    printf(" %s\t% 8ld %s",ConvierteModo2(fs.st_mode),fs.st_size,lastDir(tr));
      	    
    //-link option
  	if(!pr->lnk){
      printf("\n");
    }else{
      if(pr->lnk && (readlink(copy,symBuff,MAX) != -1))  
    	printf(" ->%s\n",symBuff);
    	else printf(" ->\n");
    }
         	    
    } else printf("% 8ld  %s\n", fs.st_size,lastDir(tr));
   }
}

bool isDirectory(char * dir) {
    struct stat fs;

    if(lstat(dir, &fs)==-1){
        return 0;
    }
    return S_ISDIR(fs.st_mode);
}

void auxPrintFiles(char *dir, char *name,struct statParams *pr) {
    struct stat fs;
    char aux[MAX];
    
    snprintf(aux, MAX, "%s/%s", dir, name); //concatenate directory and inner file or directory

    if(!lstat(aux, &fs)){           
        if(pr->lon) printStats(aux,pr); //if long option is included, we print stats in that format
        else printf("%10ld  %s\n",fs.st_size, name);
    }else printf("%s\n", strerror(errno));             
}

void printFiles(char * dir, struct statParams *pr) {

    DIR *direct;
    struct dirent *d; 
    
    if((direct=opendir(dir)) != NULL){
      
        while ((d = readdir (direct)) != NULL){ //iterate through directory        
           if(!(pr->hid)){ 
           
             if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, ".")) continue; // skip "." and ".."
             if(!(pr->hid) && d->d_name[0]=='.') continue; //in case there is not -hid
           
             auxPrintFiles(dir,d->d_name,pr);
           }else auxPrintFiles(dir,d->d_name,pr);
       }
        closedir(direct);
   }   
}

void traverseOut(char *tr,struct statParams *pr, char * original) { //auxiliary function for -recb

    DIR *direct;
    struct dirent *d;
    char aux[MAX];

    if((direct=opendir(tr)) != NULL){
    
        while ((d = readdir (direct)) != NULL){ //iterate through directory               
            
            if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, "."))continue;// skip "." and ".."
            
            snprintf(aux, MAX, "%s/%s", tr, d->d_name); //concatenate directory and inner file or directory

            if(isDirectory(aux)){
                  traverseOut(aux,pr,original);
                  if(pr->hid || d->d_name[0]!='.'){ //print if the file is not hidden and hid is input
                      printf("************%s\n", aux);
                      printFiles(aux,pr);
                   }               
            }
        }
        closedir(direct);
    }else{
      fprintf(stderr, "****error al acceder a %s: ", tr);
      perror("");
    }
    if(!strcmp(tr, original)){ //print stats of first directory        
        printf("************%s\n", original);
        printFiles(original,pr);
    }
}

void traverseIn(char *tr,struct statParams *pr) { //auxiliary function for -reca
 
    DIR *direct;
    struct dirent *d;
    char aux[MAX];
    
    if((direct=opendir(tr)) != NULL){
   
          printf("************%s\n", tr);
          printFiles(tr,pr);
          
        while ((d = readdir (direct)) != NULL){ //iterate through directory

            if(!(pr->hid) && d->d_name[0]=='.') continue; //in case there is not -hid
            if(!strcmp(d->d_name, "..") || !strcmp(d->d_name, ".")) continue;// skip "." and ".."
            
            snprintf(aux, MAX, "%s/%s", tr, d->d_name); //concatenate directory and inner file or directory

            if(isDirectory(aux))
                traverseIn(aux,pr);                             
        }
        closedir(direct);
    }
}

void printLstats(char *tr, struct statParams *pr) {
    struct stat fs;      
    //lstad instead of stat because if path is a symbolic link,then the link itslef is stat-ed, not the file
    if(lstat(tr,&fs)){ 
      fprintf(stderr, "****error al acceder a %s: ", tr);
      perror("");
      return;
    }
    int total= pr->lon + pr->lnk + pr->acc + pr->hid + pr->reca + pr->recb;
    
    if(total==0){
        if(S_ISDIR(fs.st_mode)){
           printf("************%s\n", tr);
           printFiles(tr,pr);
        }else printf("%10ld  %s\n",fs.st_size, lastDir(tr));       
    }else if(!(S_ISDIR(fs.st_mode))){
        printStats(tr,pr);
    }else{
             
        if((pr->recb) == 1){ //-recb option (major priority)
            char copy[MAX];
            strcpy(copy,tr);
            traverseOut(tr,pr,copy);                    
        }else if(pr->reca && !pr->recb){ //-reca option
            traverseIn(tr,pr);
        }else printFiles(tr,pr); 
    }
}

bool isNumberPos(char* str){
   for(int j=0; j < strlen(str) ; j++){
      if(j > 0 && str[j]==45)
      	    return false;
      if(str[j]<48 || str[j]>57){
            return false;
       }
   }  
   return true;
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListM memoryList){
  void * p;
  int aux,id,flags=0777;
  struct shmid_ds s;
    

  if (tam)     /*tam distito de 0 indica crear */
    flags=flags | IPC_CREAT | IPC_EXCL;
  if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
  if ((id=shmget(clave, tam, flags))==-1)
    return (NULL);
  if ((p=shmat(id,NULL,0))==(void*) -1){
    aux=errno;
    if (tam) {
      shmctl(id,IPC_RMID,NULL);
      errno=aux;
      return (NULL);
    }
  }
  shmctl (id,IPC_STAT,&s);
  time_t t= time(NULL);
  struct tm *tm = localtime(&t);
  
  if(!insertNodeM(&memoryList, p, s.shm_segsz, tm,"shared",(int) clave,0,"")){
    printf("No es posible añadirlo a la lista de bloques shared\n");
  }
  if(tam==0){
    printf("Memoria compartida de clave %d en %p\n",(int) clave, p);
  }
  return (p);
}

void * MapearFichero (char * fichero, int protection, tListM memoryList) {
  int df, map=MAP_PRIVATE,modo=O_RDONLY;
  struct stat s;
  void *p;
    
  char *copy=strdup(fichero);		//copy for saving the file name
    
  if (protection&PROT_WRITE)
    modo=O_RDWR;
  if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1){
    free(copy);
    return NULL;
  }
  if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED){
    free(copy);
    return NULL;
  }

  time_t t= time(NULL);
  struct tm *tm = localtime(&t);
      
  insertNodeM(&memoryList, p, (size_t)s.st_size, tm,"descriptor",0,df,copy);	//insert node in the list
    
  return p;
}

void * strToPointer(char *tr){		//converts a string into a pointer
    void *p;
    sscanf(tr,"%p",&p);
    return p;
}

ssize_t LeerFichero (char *f, void *p, size_t cont){    
  struct stat s;
  ssize_t  n;  
  int df,aux;

  if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)   //Recupera los datos del fichero y lo abre
	  return -1;     
  if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	  cont=s.st_size;
  if ((n=read(df,p,cont))==-1){     //Lee los cont bytes del fichero con descriptor df y almacena los datos leídos p
	  aux=errno;
	  close(df);
	  errno=aux;
	  return -1;
  }
  close (df);
  return n;     //Devuelve el número de bytes leídos
}

ssize_t EscribirFichero(char *f, void *p, size_t cont,int overwrite) {
  ssize_t  n;
  int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

  if (overwrite)
	  flags=O_CREAT | O_WRONLY | O_TRUNC;
   
  if ((df=open(f,flags,0777))==-1)    //Abre el archivo
	  return -1;

  if ((n=write(df,p,cont))==-1){      //Escribe con bytes desde la memoria apuntada por p en el archivo con descriptor df
	  aux=errno;
	  close(df);
	  errno=aux;
	  return -1;
  }
  close (df);
  return n;       //Devuelve el número de bytes escritos
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void *cadtop(char* cadena) {
    void *p = (void*) strtoul(cadena, NULL, 16);
    return p;
}

void mem_pmap (void)
{ pid_t pid;      
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid (pid,NULL,0);
}