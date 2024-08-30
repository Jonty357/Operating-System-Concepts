#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

union semun {
    int val; /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    unsigned short *array; /* used for GETALL and SETALL */
};

// Function to perform semaphore wait
void sem_wait(int sem_id) {
    // Implement semaphore wait operation
    struct sembuf sb = {0, -1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

// Function to perform semaphore signal
void sem_signal(int sem_id) {
    // Implement semaphore signal operation
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main()
{
    key_t shm_key, sem_key,sem_key2, readcnt_key;
    int shm_id, rw_mutex, mutex,readcnt_id;
    union semun arg;
    char* data;
    int * readcount;

    if ((sem_key = ftok("repository.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    // Declare appropriate semaphores
    if ((rw_mutex = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    if ((sem_key2 = ftok("repository.c", 'L')) == -1) {
        perror("ftok");
        exit(1);
    }

    // Declare appropriate semaphores
    if ((mutex = semget(sem_key2, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    // Obtain a key for shared memory
    if ((shm_key = ftok("repository.c", 'T')) == -1) {
        perror("ftok");
        exit(1);
    }

    // Get the shared memory object
    if ((shm_id = shmget(shm_key, 1024, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory object
    data = shmat(shm_id, (void *)0, 0);
    if (data == (char *)(-1)) 
    {
        perror("shmat");
        exit(1);
    }

    // Obtain a key for shared memory
    if ((readcnt_key = ftok("repository.c", 'S')) == -1) {
        perror("ftok");
        exit(1);
    }

    // Get the shared memory object
    if ((readcnt_id = shmget(readcnt_key, 128, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory object
    readcount = shmat(readcnt_id, (void *)0, 0);
    if (readcount == (int *)(-1)) 
    {
        perror("shmat");
        exit(1);
    }

    printf("trying to acquire lock\n");

    sem_wait(mutex);

    (*readcount)++;

    if((*readcount)==1)
        sem_wait(rw_mutex);
    
    sem_signal(mutex);

    printf("Lock acquired\n");

    printf("\n%s\n\n",data);
    printf("Reading done press enter to release lock\n");
    getchar();

    sem_wait(mutex);
    
    (*readcount)--;

    if((*readcount)==0)
        sem_signal(rw_mutex);

    sem_signal(mutex);

    printf("Lock released\n");

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmdt(readcount) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}