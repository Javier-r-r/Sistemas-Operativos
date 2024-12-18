//Javier Rodriguez Rodriguez, grupo 4.2
//Miguel Corton Deben, grupo 4.2

#define MAX 1024
#define _GNU_SOURCE
#define TAMANO 2048

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
#include <sys/shm.h> 
#include <sys/wait.h>
#include <stddef.h>
#include <sys/mman.h>
#include "ficheros_list.h"
#include "comand_list.h"
#include "memory_list.h"
#include "process_list.h"

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

void procesar_comando(char *tr[], tList commandList, tListF fileList, tListM memoryList, tListP processList);
struct statParams getParams(char *tr[], struct statParams pr);
void printStats(char *tr, struct statParams *pr);
int TrocearCadena(char *cadena, char *trozos[]);
void printLstats(char *tr, struct statParams *pr);
char LetraTF (mode_t m);
bool isNumberPos(char* str);
void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListM memoryList);
void * MapearFichero (char * fichero, int protection, tListM memoryList);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void * strToPointer(char *tr);
ssize_t EscribirFichero(char *f, void *p, size_t cont,int overwrite);
void showMemory( void *addr, size_t len);
void LlenarMemoria (void *p, size_t cont, unsigned char byte);
void *cadtop(char* cadena);
void mem_pmap();
void Recursiva (int n);
void mostrarCredenciales ();
int BuscarVariable(char * var, char *e[]);
void showEnvironment(char **entorno, char * nombre_entorno);
int CambiarVariable(char * var, char * valor, char *e[]);
int getPrioExt(char* pri);
void identifyData(char* tr[], char* var[], char* prog[], int *prio, int *bg);
const char *Ejecutable (const char *s);
int OurExecvpe(const char *file, char *const argv[], char *const envp[]);