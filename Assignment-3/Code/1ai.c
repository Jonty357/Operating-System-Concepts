#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>

void fun(int n)
{
    while(n!=1)
    {
        if(n%2)
        n=3*n+1;
        else
        n/=2;
        printf("%d ",n);
    }
}

int main(int argc,char *argv[])
{
    fun(100);
    return 0;
}