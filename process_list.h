#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#include <stdbool.h>
#define PNULL NULL
#define MAX 1024

typedef struct tItemPL {
    pid_t pid;
    int uid;
    char time[MAX];
    char status[MAX];
    int index;
    char priority[MAX];
    int out;
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

bool insertNodeP(tListP *P, pid_t pid, int uid; char time[MAX], char status[MAX], int index, char priority[MAX]);

void removeElementP(tPosPL p, tListP *P); //Elimina un proceso

void deleteAtPositionP(tPosPL p, tListP *P);

void freeListP(tListP P);

tItemPL getData(tPosPL p);

void updateListP(tPosPL p, tListP *P);

#endif