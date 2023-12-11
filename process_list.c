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
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (sen==sigstrnum[i].senal)
		return sigstrnum[i].nombre;
 return ("SIGUNKNOWN");
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

bool isEmptyListP(tListP P) {
    return (P->next == PNULL);
}

tPosPL firstP(tListP P) {
    return P->next;
}

tPosPL lastP(tListP P) {
    tPosPL p;
    for(p=P->next; p->next!=PNULL; p=p->next);
    return p;
}

tPosPL nextP(tPosPL p, tListP P) {
    return(p->next);
}

bool insertNodeP(tListP *P, int pid, int uid, char *time, char status[MAX], int sign, char *command, int priority) {
    tPosPL m, p;

    if(!createNodeP(&m)) return false;
    else {
        m->next=PNULL;
        m->data.pid=pid;
        m->data.uid=uid;
        strcpy(m->data.time,time);
        strcpy(m->data.status,status);
        m->data.sign=0;
        strcpy(m->data.command,command);
        m->data.priority=priority;
    }
    if((*P)->next == PNULL){
        (*P)->next=m;
        return true;
    } else {
        for(p=*P; p->next!=PNULL; p=p->next)
            m->data.sign++;
        p->next=m;
        return true;
    }
    /*
    tPosP q,r; struct passwd *p;
    char fecha[MAX];
    char* formato = "%y/%m/%d %H:%M:%S";
    uid_t user;
    //se crea un nodo (si es posible)
    if (!createNodeP(&q))    return false;
    else{
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        strftime(fecha, MAX, formato, local);
        q->time = strdup(fecha);
        q->pid = pid;
        user = getuid();
        p = getpwuid(user);
        q->owner = strdup(p->pw_name);
        q->prio = getpriority(PRIO_PROCESS, q->pid);
        q->comm = strdup(comm);
        strcpy(q->status, "ACTIVO");
        q->sign = 0;
        q->next = NULL;
        if(*P==NULL) *P=q;
        else {
            for (r = *P; r->next != NULL; r = r->next); //move to end
            r->next=q;
        }return true;
    }
    */
}

/*
void removeElementP(tPosPL p, tListP *P) {
    if(p->next != PNULL)
        (*P)->next=p->next;
    if(p->next == PNULL)
        (*P)->next=PNULL;
    free(P);
}
*/
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
/*
void deleteListP(tListP *P) {
    tPosPL p, aux;

    while((*P)->next != PNULL) {
        p=(*P)->next;
        aux=p;
        p=p->next;
        removeElementP(aux,P);
    }
}
*/
void deleteListP(tListP *P){
    while(*P != NULL){
        removeElemP(*P, P);
    }
}

tItemPL getData(tPosPL p) {
    return p->data;
}

void updateListP(tPosPL p, tListP *P) {
        
    if(waitpid(p->data.pid,&(p->data.sign), WNOHANG|WUNTRACED|WCONTINUED) == p->data.pid) {
        if(WIFEXITED(p->data.sign)){    
            strcpy(p->data.status, "TERMINADO");
            p->data.sign = WEXITSTATUS(p->data.sign);                       
        }else if(WIFSIGNALED(p->data.sign)){            
            strcpy(p->data.status, "SENALADO");
            p->data.sign = WTERMSIG(p->data.sign);
        }else if(WIFSTOPPED(p->data.sign)){
            strcpy(p->data.status, "PARADO");
            p->data.sign = WTERMSIG(p->data.sign);
        }else if(WIFCONTINUED(p->data.sign)){
            strcpy(p->data.status, "ACTIVO");
            p->data.sign = 0;
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
