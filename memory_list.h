#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H

#include <stdbool.h>
#include <stddef.h>
#define MNULL NULL
#define MAX 1024


typedef struct tItemML{
    void* address;
    size_t size;
    struct tm *time;
    char *typeOfAllocation;
    int key;
    int fd;
    int index;
    char *nameOfFile;
    
}tItemML;

typedef struct tNodeM *tPosML;
struct tNodeM{
    tItemML data;
    tPosML next;
};
typedef struct tNodeM *tListM;


void createListM(tListM *L);

bool isEmptyListM(tListM L);

tPosML firstM(tListM L);

tPosML lastM(tListM L);

tPosML nextM(tPosML p, tListM L);

bool insertNodeM(tListM *L, void *q, size_t s, struct tm *t,char *type, int k, int fileDescriptor, char *name);

void removeElementM(tPosML p, tListM *L);

void deleteAtPositionM(tPosML p,tListM *L);

void printListM(tListM L);

void printListMalloc(tListM L);

void printListShared(tListM L);

void printListMmap(tListM L);

void freeListM(tListM *L);

#endif 
