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
