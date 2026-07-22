#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        /* Child: simulates a request handler that hangs */
        printf("Child Process (PID: %d) started. Simulating heavy load...\n", getpid());
        sleep(10);
        printf("Child finished naturally.\n");
        exit(0);
    } else {
        /* Parent: monitors the child */
        int status;
        printf("Parent monitoring Child (PID: %d). Waiting 3 seconds...\n", pid);
        sleep(3);

        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            /* Child still running after timeout -> treat as unresponsive */
            printf("Child (PID: %d) is unresponsive. Terminating...\n", pid);
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0); /* reap to avoid zombie */
            printf("Child successfully terminated and reaped.\n");
        } else {
            printf("Child finished successfully before timeout.\n");
        }
    }
    return 0;
}
