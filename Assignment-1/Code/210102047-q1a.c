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
    int n;
    
    if(argc==1)
    {
        int ar[1];
        ar[0]=100;
        argc=2;
        fun(ar[0]);
    }
    else
    {
        int ar[argc-1];
        for(int i=0;i<(argc-1);i++)
        ar[i]=atoi(argv[i+1]);
        for(int i=0;i<(argc-1);i++)
        {
            fun(ar[i]);
        }
    }
    
    return 0;
}