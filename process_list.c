#include "process_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

bool createNodeP(tPosPL *p) {
    *p=malloc(sizeof (struct tNodeP));
    return (*p != PNULL);
}

void createListP(tPosPL *P) {
    tPosPL p;
    if(createNodeP(&p)) {
        *P = p;
        (*P)->next = PNULL;
    }
}

bool isEmptyListP(tListP P) {
    return (L->next == PNULL);
}

tPosPL firstP(tListP P) {
    return P->next;
}

tPosPL lastP(tListP P) {
    tPosPL p;
    for(p=L->next; p->next!=PNULL; p=p->next);
    return p;
}

tPosPL nextP(tPosPL p, tListP P) {
    return(p->next);
}

bool insertNodeP(tListP *P, pid_t pid, struct tm *t, char *status, , char *priority) {
    tPosPL m, p;

    if(!createNodeP(&m)) return false;
    else {
        m->next=PNULL;
        m->
    }
}