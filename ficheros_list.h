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

char *NombreFicheroDescriptor(int descriptor, tListF L);

void createListF(tListF *L);

bool isEmptyListF(tListF L);

tPosF firstF(tListF L);

tPosF lastF(tListF L);

tPosF nextF(tPosF p, tListF L);

bool insertElementF(tItemF item, tListF *L);

void removeElementF(int df, tListF *L);

void printListF(tListF L);

void freeListF(tListF *L);

void printUntilNF(tListF L, int n);

tItemF getItemF(char *file, tListF L);

int countItemsF(tListF L);


#endif //FILE_LIST