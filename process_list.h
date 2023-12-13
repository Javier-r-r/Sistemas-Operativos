#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/shm.h>
#include  <sys/ipc.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#define PNULL NULL
#define MAX 1024

typedef struct tItemPL {
    int pid;
    char *usuario;
    char *time;
    char status[MAX];
    int sign;
    char *command;
    int priority;
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
    
bool insertNodeP(tListP *P, int pid, char *usuario, char *time, char status[MAX], int sign, char *command, int priority);

void removeElementP(tPosPL p, tListP *P); //Elimina un proceso

void removeTermP(tListP *P);

void removeSigP(tListP *P);

void deleteAtPositionP(tPosPL p, tListP *P);

void deleteListP(tListP *P);

tItemPL getData(tPosPL p);

void updateListP(tPosPL p, tListP *P);

void printListP(tListP P);

int forkaux();

void freeListP(tListP *L);

tPosPL searchPid(int pid, tListP P);

void printJob(int pid, tListP P);

#endif