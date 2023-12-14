#include "process_list.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

struct SEN {
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[]={   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL,-1},
	};    /*fin array sigstrnum */

int ValorSenal(char * sen)  /*devuelve el numero de senial a partir del nombre*/ 
{ 
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	    if (!strcmp(sen, sigstrnum[i].nombre))
		    return sigstrnum[i].senal;
    return -1;
}

char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/ 
{			/* para sitios donde no hay sig2str*/
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++) {
        if (sen==sigstrnum[i].senal)
	        return sigstrnum[i].nombre;
        if(sen == 0) return "000";      //Añadido para que aparezca el número como en el shell
    }
    return ("SIGUNKNOWN");
}

void setStat(tPosPL p){
    int status;
    if(waitpid(p->data.pid, &status, WUNTRACED|WCONTINUED|WNOHANG) == p->data.pid){
        if(WIFEXITED(status)){
            p->data.sign = WEXITSTATUS(status);
            strcpy(p->data.status, "TERMINADO");
        }if(WIFSIGNALED(status)){
            p->data.sign = WTERMSIG(status);
            strcpy(p->data.status, "SENALADO");
        }if(WIFSTOPPED(status)){
            p->data.sign = WSTOPSIG(status);
            strcpy(p->data.status, "PARADO");
        }if(WIFCONTINUED(status)){
            strcpy(p->data.status, "ACTIVO");
        }p->data.priority = getpriority(PRIO_PROCESS, p->data.pid);
    }
}

void updateList(tListP *P){
    tPosPL p = *P;
    while(p!=NULL){
        setStat(p);
        p=p->next;
    }
}

bool createNodeP(tPosPL *p) {
    *p=malloc(sizeof (struct tNodeP));
    return (*p != PNULL);
}

void createListP(tPosPL *P) {
    tPosPL p;
    if(createNodeP(&p)) {
        *P = p;
        (*P)->next = PNULL;
    }
}

bool insertElementP(int pid, char* comm, tListP *P){
    tPosPL q,r; struct passwd *p;
    char fecha[MAX];
    char* formato = "%Y/%m/%d %H:%M:%S"; //La Y (mayúscula) para que aparecan los 4 dígitos del año
    uid_t user;
    //se crea un nodo (si es posible)
    if (!createNodeP(&q))    return false;
    else{
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        strftime(fecha, MAX, formato, local);
        q->data.time = strdup(fecha);
        q->data.pid = pid;
        user = getuid();
        p = getpwuid(user);
        q->data.usuario = strdup(p->pw_name);
        q->data.priority = getpriority(PRIO_PROCESS, q->data.pid);
        q->data.command = strdup(comm);
        strcpy(q->data.status, "ACTIVO");
        q->data.sign = 0;
        q->next = NULL;
        if(*P==NULL) *P=q;
        else {
            for (r = *P; r->next != NULL; r = r->next); //move to end
            r->next=q;
        }return true;
    }
}

void removeElemP(tPosPL p, tListP *P) {       //Borra un elemento en una posición concreta
    tPosPL q;
    if (p==*P) {
        *P = (*P)->next;
    } else if (p->next == NULL) {
        for (q=*P; q->next!=p; q=q->next);
        q->next = NULL;
    } else {
        for (q=*P; q->next!=p; q=q->next);
        q->next = p->next;
    }
    free(p->data.time);
    free(p->data.command);
    free(p);
}

void removeTermP(tListP *P) {       //Elimina procesos ya terminados
    tPosPL p = *P;
    tPosPL aux;
    while (p!=NULL) {
        aux = p->next;
        if(strcmp(p->data.status, "TERMINADO")==0) removeElemP(p, P);
        p = aux;
    }
}

void removeSigP(tListP *P){         //Elimina procesos terminados por señal
    tPosPL p = *P;
    tPosPL aux;
    while (p!=NULL) {
        aux = p->next;
        if(strcmp(p->data.status, "SENALADO")==0) removeElemP(p, P);
        p = aux;
    }
}

void deleteAtPositionP(tPosPL p, tListP *P) {
    tPosPL aux;
    if(p == ((*P)->next))
        (*P)->next=p->next;
    else if(p->next == PNULL) {
        for(aux=(*P)->next; aux->next!=p; aux=aux->next);
        aux->next=PNULL;
    } else {
        for(aux=(*P)->next; aux->next!=p; aux=aux->next);
        aux->next=p->next;
    }
    free(p);
}

void deleteListP(tListP *P){
    while(*P != NULL){
        removeElemP(*P, P);
    }
}

void printListP(tListP P){
    updateList(&P);
    if(P!=NULL) {
        tPosPL p = P;
        while(p!=NULL){
            printf("%6d %s p=%d %s %s (%3s) %s\n", p->data.pid, p->data.usuario, p->data.priority, p->data.time, p->data.status, NombreSenal(p->data.sign), p->data.command);
            p=p->next;
        }
    }
}

int forkaux(){
	pid_t pid, procpid;
	if ((pid=fork())==0){
        procpid = getpid();
		printf ("ejecutando proceso %d\n", procpid);
        return procpid;
	}else if (pid!=-1){
		waitpid (pid,NULL,0);
        return 0;
    } return -1;
}

void freeListP(tListP *L){
    tPosPL p, aux;
    
    while((*L)->next != PNULL){
        p = (*L)->next;
        aux = p;
        p=p->next;
        deleteAtPositionP(aux,L);
    }
}

tPosPL searchPid(int pid, tListP P){
    tPosPL p = P;
    while(p!=PNULL){
        if(p->data.pid == pid)
            return p;
        p=p->next;
    }return NULL;
}

void printJob(int pid, tListP P){
    updateList(&P);
    tPosPL p;
    if((p = searchPid(pid, P))!=NULL)
        printf("%6d %s p=%d %s %s (%3s) %s\n", p->data.pid, p->data.usuario, p->data.priority, p->data.time, p->data.status, NombreSenal(p->data.sign), p->data.command);
}