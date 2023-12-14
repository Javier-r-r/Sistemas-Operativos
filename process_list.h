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

int ValorSenal(char * sen);
char *NombreSenal(int sen);
void createListP(tListP *P);
void setStat(tPosPL p);
void updateList(tListP *P);
bool createNodeP(tPosPL *p);
void createListP(tPosPL *P);
bool insertElementP(int pid, char* comm, tListP *P);
void removeElemP(tPosPL p, tListP *P);
void removeTermP(tListP *P);
void removeSigP(tListP *P);
void deleteAtPositionP(tPosPL p, tListP *P);
void deleteListP(tListP *P);
void printListP(tListP P);
int forkaux();
void freeListP(tListP *L);
tPosPL searchPid(int pid, tListP P);
void printJob(int pid, tListP P);

#endif