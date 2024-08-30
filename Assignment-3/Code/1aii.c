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
    while(1)
    {
        scanf("%d",&n);
        if(n==1)
        break;
        else
        fun(n);
    }
    return 0;
}