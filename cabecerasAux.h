//Javier Rodriguez Rodriguez, grupo 4.2
//Miguel Corton Deben, grupo 4.2

#define MAX 1024
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stdbool.h>
#include "ficheros_list.h"
#include "comand_list.h"

struct cmd {
  char *nombre;
  void (*pfuncion) (char **);
};

struct statParams{
  int lon;
  int acc;
  int lnk;
  int hid;
  int reca;
  int recb;
};

void procesar_comando(char *tr[], tList commandList, tListF fileList);
struct statParams getParams(char *tr[], struct statParams pr);
void printStats(char *tr, struct statParams *pr);
int TrocearCadena(char *cadena, char *trozos[]);
void printLstats(char *tr, struct statParams *pr);
char LetraTF (mode_t m);