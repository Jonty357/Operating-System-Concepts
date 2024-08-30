#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>
#include<unistd.h>
#include<wait.h>

// Define a message structure
struct message {
    long mtype;
    int row;
    int col;
};

 

int main() {

    FILE * f1=fopen("ML2-input.txt","r");
    FILE * f2=fopen("temp.txt","a");
    int m;
    int n;

    fscanf(f1,"%d %d",&m,&n);
    
    int Kx[m][n];
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            fscanf(f1,"%d",&Kx[i][j]);
        }
    }

    fscanf(f1,"%d %d",&m,&n);

    int Ky[m][n];
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            fscanf(f1,"%d",&Ky[i][j]);
        }
    }

    fscanf(f1,"%d %d",&m,&n);

    int I[m][n];

    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            fscanf(f1,"%d",&I[i][j]);
        }
    }

    

    // Obtain a unique key
    key_t key = ftok("ML2-input.txt", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

 

    // Get access to the message queue of the 1st program
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    int child=0;

    // Loop to receive and process messages
    for (int i = 1; i <= 256 * 256; i++) {
        struct message msg;
        msgrcv(msgid, &msg, sizeof(struct message), 1, 0);
        printf("%d %d\n",msg.row,msg.col);

        if(child==8)
        {
            wait(NULL);
            child--;
        }

        child++; 

        int status=fork();
        if(status==0)
        {
            double gx,gy;
            gx=0;
            for(int i=-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    gx+=I[msg.row+i][msg.col+j]*Kx[i+1][j+1];
                }
            }
            gy=0;
            for(int i=-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    gy+=I[msg.row+i][msg.col+j]*Ky[i+1][j+1];
                }
            }
            double G=sqrt(gx*gx+gy*gy);
            if(G>=255) G=255;

            
            fprintf(f2,"(%d,%d): %lf\n",msg.row,msg.col,G);

            exit(0);
        }
        
    }

    fclose(f2);

    double gmat[257][257];

    // Sort the output file as per the specified order
    int a,b;
    double val;
    f2=fopen("temp.txt","r");
    for(int i=1;i<=256*256;i++)
    {
        fscanf(f2,"(%d,%d): %lf\n",&a,&b,&val);
        gmat[a][b]=val; 
    }
    fclose(f2);
    f2=fopen("temp.txt","w");
    for(int i=1;i<=256;i++)
    {
        for(int j=1;j<=256;j++)
        {
            fprintf(f2,"(%d,%d): %lf\n",i,j,gmat[i][j]);
        }
    }
    fclose(f2);
    fclose(f1);
    return 0;
}