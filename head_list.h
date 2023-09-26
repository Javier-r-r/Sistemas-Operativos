//Javier Rodriguez Rodriguez, grupo 
//Miguel Cortón Debén, grupo
#ifndef HIST_LIST
#define HIST_LIST

#include <stdbool.h>
#define LNULL NULL
#define MAX 1024


typedef struct tItemL{
    char comando[MAX];
    int index;
}tItemL;

typedef struct tItemF{
  int descriptor;
  int modo;
  char nombre[256];
}tItemF;

typedef struct tNodeF *tPosF;
typedef struct tNode *tPosL;

struct tNodeF{
  tItemF file;
  tPosL next;
};
typedef struct tNodeF *tListF;

struct tNode{
    tItemL data;
    tPosL next;
};
typedef struct tNode *tList;

void procesar_comando(char *arg, tList *commandList);

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


#endif //ASSIG1_LIST_H
