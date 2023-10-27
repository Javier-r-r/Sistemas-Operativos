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

    if (mode == -1) 
        perror("Error modeToString\n");
    else if (mode == O_CREAT)
        result = "O_CREAT";
    else if (mode == O_EXCL)
        result = "O_EXCL";
    else if (mode == O_RDONLY)
        result = "O_RDONLY";
    else if (mode == O_WRONLY)
        result = "O_WRONLY";
    else if (mode == O_RDWR)
        result = "O_RDWR";
    else if (mode == O_APPEND)
        result = "O_APPEND";
    else if (mode == O_TRUNC)
        result = "O_TRUNC";
    else
        result = "";
    
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

void removeElementF(int df, tListF *L) {
    tPosF current = firstF(*L);
    tPosF previous = FNULL;

    while (current != FNULL) {
        if (current->file.descriptor == df) {
            if (previous == FNULL) {
                // Si no hay elemento anterior, significa que es el primer elemento
                (*L)->next = current->next;
                if ((*L)->next == FNULL) {
                    // Si el primer elemento era el único en la lista, actualiza *L a FNULL
                    *L = FNULL;
                }
            } else {
                previous->next = current->next;
            }

            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

//Funcion para intercambiar dos elementos de la lista
void swap(tPosF a, tPosF b) {
    tItemF temp = a->file;
    a->file = b->file;
    b->file = temp;
}

// Función para ordenar la lista en función del descriptor
void sortListF(tListF *L) {
    int swapped;
    tPosF ptr1;
    tPosF lptr = FNULL;

    if (*L == FNULL)
        return;

    do {
        swapped = 0;
        ptr1 = (*L)->next;

        while (ptr1->next != lptr) {
            if (ptr1->file.descriptor > ptr1->next->file.descriptor) {
                // Intercambia los elementos si el descriptor es mayor
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void printListF(tListF L){
    if (firstF(L) != FNULL) {
        sortListF(&L);
        tPosF p=L->next;
       
        while(p != FNULL){
            printf("descriptor: %d -> %s %s\n",p->file.descriptor,p->file.nombre, modeToString(p->file.mode));
            p=p->next;
        }
    }
}

void printUntilNF(tListF L, int n) {
    tPosF p=L->next;

    while (p != FNULL){
    	if(p->file.index < n)
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

bool existsDescriptor(int df, tListF L) {
    tPosF p;
    for (p = firstF(L); p != NULL; p = nextF(p, L)) {
        if (p->file.descriptor == df) {
            return true; // Se encontró el descriptor en la lista
        }
    }
    return false; // El descriptor no se encontró en la lista
}
