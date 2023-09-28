//Javier Rodriguez Rodriguez
//Miguel Cortón Debén
#include "ficheros_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

char *NombreFicheroDescriptor(int descriptor, tListF L) {
  tPosF current = firstF(L);

  while (current != NULL) {
    if (current->file.descriptor == descriptor) {
      return current->file.nombre;
    }
      current = current->next;
  }

  return NULL; // Si no se encuentra el descriptor, devolvemos NULL
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
            printf("%4d-> %s\n",p->file.index+1,p->file.nombre);
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

void printUntilNF(tListF L, int n){
    tPosF p=L->next;

    while (p != FNULL){
    	if(p->file.index < n)
        printf("%4d-> %s\n",p->file.index+1,p->file.nombre);
        p=p->next;
    }
}

tItemF getItemF(char *file, tListF L){
    tPosF q;
    for(q=L->next;q->file.nombre!=file;q=q->next);
    return q->file;
}

int countItemsF(tListF L) {
  int count = 0;
  tPosF p;
  for (p = firstF(L); p != NULL; p = nextF(p, L))
    count ++;
  return count;
}

