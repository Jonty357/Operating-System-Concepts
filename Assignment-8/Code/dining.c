#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>

// Define semaphore and shared memory keys
key_t sem_key[5];
key_t shm_key[5];
key_t shm_key_cnt;

// Define semaphore and shared memory identifiers
int sem_id[5];
int shm_id[5];
int shm_id_cnt;

// Define the database table structure
struct DatabaseTable {
    char name[50];
    int pid[10];
};

union semun {
    int val; /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    unsigned short *array; /* used for GETALL and SETALL */
};

// Implement sem_wait function
void sem_wait(int sem_index) {
    // Initialize sembuf structure and perform semop
    // You need to use semop to perform wait operation on the semaphore
    struct sembuf sb = {0, -1, 0};
    if (semop(sem_index, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

// Implement sem_signal function
void sem_signal(int sem_index) {
    // Initialize sembuf structure and perform semop
    // You need to use semop to perform signal operation on the semaphore
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_index, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int philosopher(int philosopher_index,int round) {
    // Define which tables this philosopher can access
    int left_table = (philosopher_index+round)%5;
    int right_table = (philosopher_index + round +1) % 5;
    

    
        // Philosophers pick two adjacent tables (equivalent to picking two forks)
        // Acquire semaphores
        if(philosopher_index%2)
        {
            sem_wait(left_table);
            sem_wait(right_table);
        }
        else
        {
            sem_wait(right_table);
            sem_wait(left_table); 
        }

        printf("Transaction %d holding the semaphores.\n", getpid());

        // Perform a transaction
        struct DatabaseTable *table1 = shmat(shm_id[left_table], NULL, 0);
        struct DatabaseTable *table2 = shmat(shm_id[right_table], NULL, 0);
        printf("Transaction %d: operating on %s - %s.\n", getpid(), table1->name, table2->name);
        int index;
        for(int l=0;l<10;l++)
        {
            if(table1->pid[l]==-1)
            {
                index=l;
                break;
            }
        }
        table1->pid[index]=getpid();
        for(int l=0;l<10;l++)
        {
            if(table2->pid[l]==-1)
            {
                index=l;
                break;
            }
        }
        table2->pid[index]=getpid();

        // Sleep for 10 seconds
        sleep(10);

        int *count=shmat(shm_id_cnt,NULL,0);
        (*count)++;

        // Release semaphores
        if(philosopher_index%2)
        {
            sem_signal(left_table);
            sem_signal(right_table);
        }
        else
        {
            sem_signal(right_table);
            sem_signal(left_table); 
        }

        printf("Transaction %d released the semaphores.\n", getpid());
        shmdt(table1);
        shmdt(table2);

    return 0;
}

int main() {
    // Obtain semaphore keys
    for (int i = 0; i < 5; i++) {
        sem_key[i] = ftok("dining.c", i);
    }

    // Declare and initialize semaphores
    for (int i = 0; i < 5; i++) {
        sem_id[i] = semget(sem_key[i], 1, IPC_CREAT | 0666);
        semctl(sem_id[i], 0, SETVAL, 1);
    }

    // Obtain shared memory keys
    for (int i = 0; i < 5; i++) {
        shm_key[i] = ftok("dining.c", i+10);
    }
    shm_key_cnt = ftok("dining.c", 'Z');

    // Create and attach shared memory
    for (int i = 0; i < 5; i++) {
        shm_id[i] = shmget(shm_key[i], sizeof(struct DatabaseTable), IPC_CREAT | 0666);
        struct DatabaseTable *table = shmat(shm_id[i], NULL, 0);
        
        // Initialize the table with table number
        snprintf(table->name, sizeof(table->name), "Table %d", i+1);
        for (int j = 0; j < 10; j++) {
            table->pid[j] = -1;
        }
        shmdt(table);
    }
    shm_id_cnt=shmget(shm_key_cnt,sizeof(int), IPC_CREAT | 0666);
    int *count=shmat(shm_id_cnt,NULL,0);
    *count=0;

    // Create five children (philosophers)
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Philosopher's logic here
            for(int j=0;j<5;j++)
            {
            philosopher(i,j);
            int *count=shmat(shm_id_cnt,NULL,0);
            while(*count<5*(j+1));
            }
            exit(0);
        }
    }
    for(int i=0;i<5;i++)
    {
        wait(NULL);
    }

    // Print the contents of every table
    for(int k=0;k<5;k++)
    {
        struct DatabaseTable *table1 = shmat(shm_id[k], NULL, 0);
        printf("%s\n",table1->name);
        for(int j=0;j<10;j++)
        {
            printf("%d ",table1->pid[j]);
        }
        printf("\n");

        shmdt(table1);
    }

    for(int i=0;i<5;i++)
    {
        shmctl(shm_id[i],IPC_RMID,NULL);
    }

    shmdt(count);
    shmctl(shm_id_cnt,IPC_RMID,NULL);

    return 0;
}
