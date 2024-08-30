#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <sys/time.h>

// Function to start the daemon
void startDaemon() {
    // Check if the daemon is already running
    
    struct rlimit rl;

    pid_t p; // (a)(i) Creating child process
    p=fork();
    if(p!=0)
        exit(-1);

    umask(0); // (a)(ii)(i) Calling umask function

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0){
        perror("My Daemon: can't get file limit");
        exit(1);
    }

    FILE * f=fopen("config-210101114.txt","w");
    
    pid_t pid = fork(); // (a)(ii)(ii) Creation of child using fork
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    fprintf(f,"%d",pid);

    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    }

    // (a)(ii)(iii) Create a new session
    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    printf("child created");

    // (a)(ii)(iv) Change working directory to root
    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // (a)(ii)(v) Close all file descriptors
    int i;
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    // (a)(ii)(vi) Initialize the log file
    openlog("MyDaemon", LOG_PID, LOG_DAEMON);

    // (a)(iii) Write a message to syslog
    syslog(LOG_NOTICE, "Daemon started successfully");

    // (a)(iv) Main daemon logic
    while (true) {
        pid_t child_pid = fork();
        if (child_pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        if (child_pid == 0) {
            // Child process
            int n = getpid();
            while (n > 1) {
                syslog(LOG_INFO, "Sequence: %d", n); // (a)(v) appending to syslog file
                if (n % 2 != 0) {
                    n = 3 * n + 1;
                    
                } else {
                    n /= 2;
                }
            }
            syslog(LOG_INFO, "Sequence: %d", n);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            sleep(20); // Wait for 20 seconds before creating another child
        }
    }
}

// Function to stop the daemon
void stopDaemon(pid_t pid) {
    if (kill(pid, SIGTERM) < 0) {
        perror("Kill failed");
        exit(EXIT_FAILURE);
    }
    syslog(LOG_NOTICE, "Daemon stopped"); // (b)(ii) Message in syslog file aout stoping of daemon
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <start|stop>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "start") == 0) // (c) invoke start_daemon
    {
        printf("starting new daemon\n");
        startDaemon();
    } 
    else if (strcmp(argv[1], "stop") == 0) // (d) invoke stop_daemon
    {
        pid_t daemon_pid; /* (b)(i) Get the PID of the running daemon by reading from the file */
        FILE * f1=fopen("config-210101114.txt","r"); 
        fscanf(f1,"%d",&daemon_pid);
        stopDaemon(daemon_pid);
    } 
    else 
    {
        printf("Usage: %s <start|stop>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}