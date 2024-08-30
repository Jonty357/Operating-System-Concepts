#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>
#include<string.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc,char *argv[])
{
    char s[100],s1[100],s2[100],s3[100];
    char ch;
    int i=0;
    while(1)
    {
        printf("Enter command : ");
        while((ch=getchar())!='\n')
        {
            s[i++]=ch;
        }
        s[i]='\0';
        if(strcmp(s,"quit")==0)
        exit(0);
        int j=0;
        while(s[j]!='|')
        {
            s1[j]=s[j];
            j++;
        }
        s1[j]='\0';
        int k,l;
        k=0;
        j++;
        while(s[j]!='|')
        {
            s2[k++]=s[j++];
        }
        s2[k]='\0';
        j++;
        k=0;
        while(s[j]!='\0')
        {
            s3[k++]=s[j++];
        }
        s3[k]='\0';
        
        // printf("%s\n",s1);
        // printf("%s\n",s2);
        // printf("%s\n",s3);
        int fd[2];
        pid_t pid;

        int _out,_in;

        dup2(1,_out);
        dup2(0,_in);

        if(pipe(fd)==-1)
        {
            fprintf(stderr,"Pipe failed");
            return 1;
        }

        pid=fork();

        if(pid<0)
        {
            fprintf(stderr,"Fork failed");
            return 1;
        }

        if(pid>0)
        {
            close(fd[READ_END]);

            if (dup2(fd[WRITE_END], 1) == -1) {
                perror("dup2");
                return 1;
            }

            system(s1);

            dup2(_out,1);

            close(fd[WRITE_END]);

            wait(NULL);

        }
        else
        {
            close(fd[WRITE_END]);

            dup2(fd[READ_END],0);

            int fd1[2];

            pid_t pid1;                

                if(pipe(fd1)==-1)
                {
                    fprintf(stderr,"Pipe failed");
                    return 1;
                }

                pid1=fork();

                if(pid1<0)
                {
                    fprintf(stderr,"Fork failed");
                    return 1;
                }

                else if(pid1>0)
                {
                    close(fd1[READ_END]);

                    if (dup2(fd1[WRITE_END], 1) == -1) {
                        perror("dup2");
                        return 1;
                    }

                    system(s2);

                    dup2(_out,1);

                    close(fd1[WRITE_END]);

                    // execlp(s2,s2,NULL);
                    wait(NULL);

                }
                else
                {
                    close(fd1[WRITE_END]);

                    dup2(fd1[READ_END],0);

                    system(s3);

                    dup2(_in,0);

                    close(fd1[READ_END]);

                    exit(0);
                    //execlp(s3,s3,NULL);
                }

            dup2(_in,0);
            close(fd1[READ_END]);
            exit(0);
        }
        i=0;
    }
    
    return 0;
}