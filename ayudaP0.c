/*las funciones entre puntos hay que implementarlas */

struct FicheroAbierto {
    int descriptor;  // Descriptor de archivo
    int modo;        // Modo de apertura (lectura, escritura, etc.)
    char nombre[256]; // Nombre del archivo
};

// Función para agregar una entrada a la tabla de archivos abiertos
void AnadirAFicherosAbiertos(struct FicheroAbierto *ficherosAbiertos, int *numFicherosAbiertos, int descriptor, int modo, const char *nombre) {
    if (*numFicherosAbiertos < 100) { // Suponemos que hay un máximo de 100 archivos abiertos
        ficherosAbiertos[*numFicherosAbiertos].descriptor = descriptor;
        ficherosAbiertos[*numFicherosAbiertos].modo = modo;
        strncpy(ficherosAbiertos[*numFicherosAbiertos].nombre, nombre, sizeof(ficherosAbiertos[*numFicherosAbiertos].nombre));
        (*numFicherosAbiertos)++;
    } else {
        printf("No se pueden abrir más archivos. La tabla de archivos abiertos está llena.\n");
    }
}

void ListarFicherosAbiertos(const struct FicheroAbierto *ficherosAbiertos, int numFicherosAbiertos) {
    printf("Ficheros abiertos:\n");

    for (int i = 0; i < numFicherosAbiertos; i++) {
        printf("Descriptor: %d, Modo: %d, Nombre: %s\n", ficherosAbiertos[i].descriptor, ficherosAbiertos[i].modo, ficherosAbiertos[i].nombre);
    }
}

void Cmd_open (char * tr[])
{
    int i,df, mode=0;
    
    if (tr[0]==NULL) { /*no hay parametro*/
        ListarFicherosAbiertos();
        return;
    }
    for (i=1; tr[i]!=NULL; i++)
      if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
      else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
      else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY; 
      else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
      else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
      else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
      else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC; 
      else break;
      
    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        ...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        printf ("Anadida entrada a la tabla ficheros abiertos..................",......);
}

void Cmd_close (char *tr[])
{ 
    int df;
    
    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
      ..............ListarFicherosAbiertos............... /*o el descriptor es menor que 0*/
      ListarFicheroAbierto();
      return;
    }

    
    if (close(df)==-1)
        perror("Imposible cerrar descriptor");
    else
       ........EliminarDeFicherosAbiertos......
       printf("El archivo ha sido eliminado\n");
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
