#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

#include "my_system.h"

int my_system(const char *command) {
    pid_t pid = fork();
    if (pid > 0) {
        // Parent
        int status;
        waitpid(pid, &status, 0);
        return status;
    } else {
        // Child
        // printf("Running '%s'\n", command);
        printf(""); // doesn't run properly without a print
        int err = execl("/bin/sh", "sh", "-c", command);
        return err;
    }
    return 0;
}