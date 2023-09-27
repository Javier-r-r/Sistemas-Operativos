//Javier Rodriguez Rodriguez
//Miguel Cortón Debén
#include "file_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

bool createNode(tPosL *p){
    *p=malloc(sizeof (struct tNode));
    return (*p != LNULL);
}


void createListF(tListF *L){
    tPosF p;
    if(createNode(&p)){
    	*L= p;
    	(*L)->next=LNULL;
    }
    
}

bool isEmptyListF(tListF L){
    return (L->next==LNULL);
}

tPosF firstF(tListF L){
    return L->next;
}

tPosF lastF(tListF L){
    tPosF p;
    for(p=L->next;p->next!=LNULL;p=p->next);
    return p;
}

tPosF nextF(tPosF p, tListF L){
    return(p->next);
}

bool insertElementF(char *file, tListF *L){
    tPosF m, p;
    if(!createNode(&m))return false;
    else{
        strcpy(m->file.nombre, data);
        m->next=LNULL;
        m->file.index=0;
    }
    if((*L)->next==LNULL){
        (*L)->next=m;
        return true;
    }else{
        for(p=*L; p->next!=LNULL;p=p->next){ 
            m->file.index++;
        }
        p->next=m;
    
    return true;
   }
}

void removeElementF(tPosF p, tListF *L){
    
    if(p->next != LNULL){
        (*L)->next=p->next; 
    }
    if(p->next ==LNULL){
        (*L)->next=LNULL;
    }
    free(p);
}


void printListF(tListF L){
    tPosF p=L->next;
       
       while(p != LNULL){
            printf("%4d) %s\n",p->file.index+1,p->file.nombre);
            p=p->next;
        }
    
}

void freeListF(tListF *L){
    tPosF p, aux;
    
    while((*L)->next != LNULL){
        p = (*L)->next;
        aux = p;
        p=p->next;
        removeElement(aux,L);
    }
}

void printUntilNF(tListF L, int n){
    tPosF p=L->next;

    while (p != LNULL){
    	if(p->file.index < n)
        printf("%4d) %s\n",p->file.index+1,p->file.nombre);
        p=p->next;
    }
}

tItemF getItemF(int n, tListF L){
    n--;
    tPosF q;
    for(q=L->next;q->file.index!=n;q=q->next);
    return q->file;
}

int countItemsF(tListF L) {
  int count = 0;
  tPosL p;
  for (p = first(L); p != NULL; p = next(p, L))
    count ++;
  return count;
}
