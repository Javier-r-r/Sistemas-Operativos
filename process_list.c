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

bool insertNodeP(tListP *P, pid_t pid, int uid; char time[MAX], char status[MAX], int index, char priority[MAX]) {
    tPosPL m, p;

    if(!createNodeP(&m)) return false;
    else {
        m->next=PNULL;
        m->data.pid=pid;
        m->data.uid=uid;
        strcpy(m->data.time,time);
        strcpy(m->data.status,status);
        m->data.index=0;
        strcpy(m->data.priority,priority);
    }
    if((*P)->next == PNULL){
        (*P)->next=m;
        return true;
    } else {
        for(p=*P, p->next!=PNULL, p=p->next)
            m->data-index++;
        p->next=m;
        return true;
    }
}

void removeElementP(tPosPL p, tListP *P) {
    if(p->next != PNULL)
        (*P)->next=p->next;
    if(p->next == PNULL)
        (*P)->next=PNULL;
    free(P);
}

void deleteAtPositionP(tPosPL p, tListP *P) {
    tPosPL aux;

    if(p == ((*P)->next))
        (*P)->next=p->next;
    else if(p->next == PNULL) {
        for(aux=(*P)->next; aux->next!=p; aux=aux->next);
        aux->next=PNULL;
    } else {
        for(aux=(*P)->next; aux->next!=p; aux=aux->next);
        aux->next=p->next;
    }
    free(p);
}

void freeListP(tListP *P) {
    tPosPL p, aux;

    while((*P)->next != PNULL) {
        p=(*P)->next;
        aux=p;
        p=p->next;
        removeElementP(aux,P);
    }
}

tItemPL getData(tPosPL p) {
    return p->data;
}

void updateListP(tPosPL p, tListP *P) {
    if(waitpid(p->data.pid,&(p->data.out) WNOHANG|WUNTRACED|WCONTINUED) == p->data.pid) {
        if(WIFEXITED(p->data.out)){                
            strcpy(p->data.status, "FINISHED");
            p->data.out = WEXITSTATUS(p->data.out);
        }else if(WIFSIGNALED(p->data.out)){
            strcpy(p->data.status, "SENALADO");
            p->data.out = WTERMSIG(p->data.out);
        }else if(WIFSTOPPED(p->data.out)){
            strcpy(p->data.status, "STOPPED");
            p->data.out = WTERMSIG(p->data.out);
        }else if(WIFCONTINUED(p->data.out)){
            strcpy(p->data.status, "ACTIVE");
            p->data.out = 0;
        }
    }
}