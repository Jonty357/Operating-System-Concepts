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
        printf("Ist progam with no arguments:\n");
        execle("210101114-q1a","210101114-q1a",NULL,NULL);
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
        printf("Ist progam with one argument:\n");
        execl("210101114-q1a","210101114-q1a","7",NULL);
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
        printf("Ist progam with 10 arguments:\n");
        char * args[]={"210101114-q1a","10","15","20","25","30","35","40","45","50","60",NULL};
        execve("210101114-q1a",args,NULL);
        exit(0);
    }
    else
    {
        wait(NULL);
    }
    
    return 0;
}