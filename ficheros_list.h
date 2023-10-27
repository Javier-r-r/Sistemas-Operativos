//Javier Rodriguez Rodriguez, grupo 
//Miguel Cortón Debén, grupo
#ifndef FILE_LIST
#define FILE_LIST

#include <stdbool.h>
#define FNULL NULL
#define MAX 1024


typedef struct tItemF{
  int mode;
  int index;
  int descriptor;
  char nombre[MAX];
}tItemF;

typedef struct tNodeF *tPosF;

struct tNodeF{
    tItemF file;
    tPosF next;
};

typedef struct tNodeF *tListF;

void createListF(tListF *L);

bool isEmptyListF(tListF L);

tPosF firstF(tListF L);

tPosF lastF(tListF L);

tPosF nextF(tPosF p, tListF L);

bool insertElementF(tItemF item, tListF *L);

void removeElementF(int df, tListF *L);

void printListF(tListF L);

void printUntilNF(tListF L, int n);

void freeListF(tListF *L);

tItemF getItemF(int df, tListF L);

bool existsDescriptor(int df, tListF L);

#endif //FILE_LIST