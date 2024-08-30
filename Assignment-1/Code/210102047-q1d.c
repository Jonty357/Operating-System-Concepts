#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
    pid_t pid;
    pid=fork();

    if(pid<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if(pid==0)
    {
        printf("2nd progam with no arguments:\n");
        execl("210101114-q1b","210101114-q1b",NULL);
        exit(0);
    }
    else
    {
        wait(NULL);
    }
    
    pid=fork();

    if(pid<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if(pid==0)
    {
        printf("2nd progam with one argument:\n");
        char * envp[]={"n=7",NULL};
        execle("210101114-q1b","210101114-q1b",NULL,envp);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    pid=fork();

    if(pid<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if(pid==0)
    {
        printf("2nd progam with 10 arguments:\n");
        char * args[]={"210101114-q1b",NULL};
        char * envp[]={"n=10 15 20 25 30 35 40 45 50 60"};
        execve("210101114-q1b",args,envp);
        exit(0);
    }
    else
    {
        wait(NULL);
    }    
    return 0;
}