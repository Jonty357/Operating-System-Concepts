#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[])
{
    while(1){
        char s[100];
        char ch;
        int i=0;

        //printf("\nEnter the command: ");
        while((ch=getchar())!='\n'){
            s[i++]=ch;
        }
        s[i]='\0';

        if(strcmp(s,"quit")==0) break;

        if(strcmp(s,"./compute_sequence_1 | ./compute_sequence_2")!=0){
            printf("Invalid command!\n");
            continue;
        }

        char left[100],right[100];
        i=0;
        while(s[i]!=' '){
            left[i]=s[i];
            i++;
        }
        left[i]='\0';
        i+=3;
        int j=0;
        while(s[i]!='\0'){
            right[j++]=s[i++];
        }
        right[j]='\0';

        int fd[2];
        pid_t pid;

        if (pipe(fd) == -1) {
            fprintf(stderr,"Pipe failed");
            return 1;
        }
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        if (pid == 0) {
            pid_t pid1=fork();

            if (pid1 < 0) {
                fprintf(stderr, "Fork Failed");
                return 1;
            }
            if (pid1 > 0) {
                close(fd[READ_END]);
                
                if(dup2(fd[WRITE_END],1)==-1){
                    perror("Error occurred\n");
                    return 1;
                }

                close(fd[WRITE_END]);

                execlp(left,left,NULL);
                wait(NULL);
            }
            else {
                close(fd[WRITE_END]);
                
                if(dup2(fd[READ_END],0)==-1){
                    perror("Error occurred\n");
                    return 1;
                }
                
                close(fd[READ_END]);

                execlp(right,right,NULL);
            
            }
        }
        else{
            execlp("./compute_sequence_3","./compute_sequence_3",NULL);
        }
    }
    
    return 0;
}