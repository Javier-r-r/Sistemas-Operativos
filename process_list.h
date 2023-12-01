#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#include <stdbool.h>
#define PNULL NULL

typedef struct tItemPL {
    pid_t pid;
    struct tm *time;
    char *status;

    char *priority;
} tItemPL;

typedef struct tNodeP *tPosPL;
struct tNodeP {
    tItemPL data;
    tPosPL next;
};

typedef struct tNodeP *tListP;

void createListP(tListP *P);

bool isEmptyListP(tListP P);

tPosPL firstP(tListP P);

tPosPL lastP(tListP P);

tPosPL nextP(tPosPL p, tListP P);

bool insertNodeP(tListP *P, pid_t pid, struct tm *t, char *status, , char *priority);

void removeElementP(tPosPL p, tListP *P); //Elimina un proceso

void deleteAtPositionP(tPosPL p, tListP *P);

void printListP(tListP P);

#endif