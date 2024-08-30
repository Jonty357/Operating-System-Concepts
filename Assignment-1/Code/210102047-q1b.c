#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>

void fun(int n)
{
    while(n!=1)
    {
        printf("%d ",n);
        if(n%2)
        n=3*n+1;
        else
        n/=2;
    }
    printf("%d\n",n);
}

int main(int argc,char *argv[])
{
    if(argc!=1)
    {
        fprintf(stderr,"This program doesnt accept arguments");
        return 1;
    }
    char * path=getenv("n");
    int n=0;

    if (path != NULL) {
        int i=0;
        while(path[i]!='\0')
        {
            if(path[i]!=' ')
            n=(n*10)+(path[i]-'0');
            else{
                fun(n);
                n=0;
            }
            i++;
        }
        if(n!=0)
        fun(n);

    } else {
        fun(100);
    }
    
    return 0;
}