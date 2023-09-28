/*las funciones entre puntos hay que implementarlas */

void Cmd_open (char *arg, tListF fileList)
{
    int i,df, mode=0;
    
    if (arg==NULL) { /*no hay parametro*/
        printListF(fileList);
        return;
    }
    char tr[MAX];
    TrocearCadena(arg, tr);
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
    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        tItemF newItem;
        newItem.descriptor = df;
        newItem.mode = mode;
        newItem.index = FNULL;
        strncpy(newItem.nombre, tr[0], MAX);
        insertElementF(newItem, fileList);
        printf("Añadida entrada a la tabla ficheros abiertos: descriptor %d, modo %d, nombre %s\n", df, mode, tr[0]);
    }
}

void Cmd_close (char *arg, tListF *fileList)
{ 
    int df;
    
    if (arg==NULL || (df=atoi(arg))<0) { /*no hay parametro*/
        printListF(fileList);
        return;
    }

    
    if (close(df)==-1)
        perror("Inposible cerrar descriptor");
    else {
        removeElementF(df);
        printf("Descriptor %d cerrado\n", df);
    }
}

void Cmd_dup (char * tr[])
{ 
    int df, duplicado;
    char aux[MAXNAME],*p;
    
    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
        ListOpenFiles(-1);                 /*o el descriptor es menor que 0*/
        return;
    }
    
 
    p=.....NombreFicheroDescriptor(df).......;
    sprintf (aux,"dup %d (%s)",df, p);
    .......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
}
