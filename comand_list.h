//Javier Rodriguez Rodriguez, grupo 
//Miguel Cortón Debén, grupo
#ifndef HIST_LIST
#define HIST_LIST

#include <stdbool.h>
#include "ficheros_list.h"
#define LNULL NULL
#define MAX 1024


typedef struct tItemL{
    char comando[MAX];
    int index;
}tItemL;

typedef struct tNode *tPosL;

struct tNode{
    tItemL data;
    tPosL next;
};
typedef struct tNode *tList;

void procesar_comando(char *tr[], tList *commandList, tListF *fileList);

void createList(tList *L);

bool isEmptyList(tList L);

tPosL first(tList L);

tPosL last(tList L);

tPosL next(tPosL p, tList L);

bool insertElement(char *data, tList *L);

void removeElement(tPosL p, tList *L);

void printList(tList L);

void freeList(tList *L);

void printUntilN(tList L, int n);

tItemL getItem(int n, tList L);

int countItems(tList L);


#endif //HIST_LIST

