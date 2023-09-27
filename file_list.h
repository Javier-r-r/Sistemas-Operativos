//Javier Rodriguez Rodriguez, grupo 
//Miguel Cortón Debén, grupo
#ifndef HIST_LIST
#define HIST_LIST

#include <stdbool.h>
#define LNULL NULL
#define MAX 1024


typedef struct tItemF{
  int mode;
  int descriptor;
  char nombre[MAX];
  int index;
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

bool insertElementF(char *file, tListF *L);

void removeElementF(tPosF p, tListF *L);

void printListF(tListF L);

void freeListF(tListF *L);

void printUntilNF(tListF L, int n);

tItemF getItemF(int n, tListF L);

int countItemsF(tListF L);


#endif //HIST_LIST