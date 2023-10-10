#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char *getoutput(const char *command) {
    // Create a pipe
    int pipe_fds[2];
    if (pipe(pipe_fds) < 0) {
        exit(1);
    }

    int pipe_read = pipe_fds[0];
    int pipe_write = pipe_fds[1];

    // Fork processes
    pid_t pid = fork();
    if (pid > 0) {
        // Parent: Read from child
        close(pipe_write);
        FILE *pipe_read_fd = fdopen(pipe_read, "r");

        // Read entire output
        size_t output_size = 1024;
        char *output = calloc(output_size + 1, sizeof(char));

        getdelim(&output, &output_size, '\0', pipe_read_fd);
        
        // Free resources
        close(pipe_read);

        int status;
        waitpid(pid, &status, 0);

        return output;
    } else {
        // Child: Run process
        dup2(pipe_write, STDOUT_FILENO); // Change stdout to pipe_out

        // Close files
        close(pipe_read);
        close(pipe_write);

        // Run the command
        // printf("Running '%s'\n", command);
        execl("/bin/sh", "sh", "-c", command, NULL);
    }

    // Free resources
    close(pipe_read);
    close(pipe_write);
    return "";
}

char *parallelgetoutput(int count, const char **argv_base) {
    // Create a shared pipe
    int pipe_fds[2];
    if (pipe(pipe_fds) < 0) {
        exit(1);
    }

    int pipe_read = pipe_fds[0];
    int pipe_write = pipe_fds[1];

    // Count arguments in array
    int argc = 0;
    while (argv_base[argc] != NULL) {
        argc += 1;
    }

    // Read entire output
    size_t output_size = 1024;
    char *output = calloc(output_size + 1, sizeof(char));

    for (int i = 0; i < count; i++) {
        // Copy over subprogram args
        const char *argv_i[argc + 2];
        for (int j = 0; j < argc; j++) {
            argv_i[j] = argv_base[j];
        }

        char prog_num[16];
        snprintf(prog_num, 16, "%d", i);
        argv_i[argc] = prog_num; // Add a number

        argv_i[argc + 1] = NULL; // Set last arg to null

        // Fork processes
        pid_t pid = fork();
        if (pid == 0) {
            // Child: Run process
            dup2(pipe_write, STDOUT_FILENO); // Change stdout to pipe_out

            // Close files
            close(pipe_read);
            close(pipe_write);

            // Run the command
            execv(argv_i[0],  (char * const *) argv_i);
        }
    }

    // Read from all children
    close(pipe_write);
    FILE *pipe_read_fd = fdopen(pipe_read, "r");
    getdelim(&output, &output_size, '\0', pipe_read_fd);
    
    // Wait for all child processes
    while (waitpid(-1, NULL, 0) != -1);

    // Free resources
    close(pipe_read);
    return output;
}