#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/msg.h>

#include <wait.h>

 

// Define a message structure

struct message {

    long mtype;

    int row;

    int col;

};

 

int main() {

    

    // Obtain a unique key

    key_t key = ftok("ML2-input.txt", 'A');

    if (key == -1) {

        perror("ftok");

        exit(1);

    }

 

    // Create a message queue

    int msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1) {

        perror("msgget");

        exit(1);

    }

 

    // Construct and send messages

    struct message msg;

    msg.mtype = 1;

 

    // Loop to send messages with (row, col)

    for (int row = 1; row <= 256; row++) {

        for (int col = 1; col <= 256; col++) {

            msg.row = row;

            msg.col = col;

            printf("%d %d\n",row,col);

            msgsnd(msgid, &msg, sizeof(struct message), 0);

            

        }

    }

 
    sleep(5);
    // Remove the message queue

    msgctl(msgid, IPC_RMID, NULL);

 

    return 0;

}

 