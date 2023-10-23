//Javier Rodriguez Rodriguez
//Miguel Cortón Debén
#include "ficheros_list.h"
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

char *modeToString(int mode) {
    char *result;
    
    switch (mode) {
        case O_RDONLY:
            result = "O_RDONLY";
            break;
        case O_WRONLY:
            result = "O_WRONLY";
            break;
        case O_RDWR:
            result = "O_RDWR";
            break;
        case O_CREAT:
            result = "O_CREAT";
            break;
        case O_EXCL:
            result = "O_EXCL";
            break;
        case O_APPEND:
            result = "O_APPEND";
            break;
        case O_TRUNC:
            result = "O_TRUNC";
            break;
        default:
            result = "Modo no válido";
            break;
    }
    
    return result;
}
bool createNodeF(tPosF *p){
    *p=malloc(sizeof (struct tNodeF));
    return (*p != FNULL);
}


void createListF(tListF *L){
    tPosF p;
    if(createNodeF(&p)){
    	*L= p;
    	(*L)->next=FNULL;
    }
    
}

bool isEmptyListF(tListF L){
    return (L->next==FNULL);
}

tPosF firstF(tListF L){
    return L->next;
}

tPosF lastF(tListF L){
    tPosF p;
    for(p=L->next;p->next!=FNULL;p=p->next);
    return p;
}

tPosF nextF(tPosF p, tListF L){
    return(p->next);
}

bool insertElementF(tItemF item, tListF *L){
    tPosF m, p;
    if(!createNodeF(&m))return false;
    else{
        m->file=item;
        m->next=FNULL;
        m->file.index=0;
    }
    if((*L)->next==FNULL){
        (*L)->next=m;
        return true;
    }else{
        for(p=*L; p->next!=FNULL;p=p->next){ 
            m->file.index++;
        }
        p->next=m;
    
    return true;
   }
}

void removeElementF(int df, tListF *L){
    
    tPosF current = firstF(*L);
    tPosF previous = NULL;

    while (current != NULL) {
        if (current->file.descriptor == df) {
            // El elemento actual tiene el descriptor que queremos eliminar
            if (previous == NULL) {
                // Si no hay elemento anterior, significa que es el primer elemento
                *L = current->next;
            } else {
                // Si hay un elemento anterior, enlazamos el anterior con el siguiente
                previous->next = current->next;
            }

            // Liberamos la memoria del elemento actual
            free(current);
            return;
        }

        previous = current;
        current = current->next;
    }
}



void printListF(tListF L){
    tPosF p=L->next;
       
       while(p != FNULL){
            printf("descriptor: %d -> %s %s\n",p->file.descriptor,p->file.nombre, modeToString(p->file.mode));
            p=p->next;
        }
    
}

void removeElementAux(tPosF p, tListF *L){
    
    if(p->next != FNULL){
        (*L)->next=p->next; 
    }
    if(p->next ==FNULL){
        (*L)->next=FNULL;
    }
    free(p);
}

void freeListF(tListF *L){
    tPosF p, aux;
    
    while((*L)->next != FNULL){
        p = (*L)->next;
        aux = p;
        p=p->next;
        removeElementAux(aux,L);
    }
}

tItemF getItemF(int df, tListF L){
    tPosF q;
    for(q=L->next;q->file.descriptor!=df;q=q->next);
    return q->file;
}

int countItemsF(tListF L) {
  int count = 0;
  tPosF p;
  for (p = firstF(L); p != NULL; p = nextF(p, L))
    count ++;
  return count;
}

