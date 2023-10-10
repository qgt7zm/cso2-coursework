#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    pid_t pid = fork();
    if (pid > 0) {
        // Parent: Read from child
        close(pipe_write);
        FILE *pipe_read_fd = fdopen(pipe_read, "r");

        // Read entire output
        size_t output_size = 1024;
        char *output = calloc(output_size + 1, sizeof(char));

        getdelim(&output, &output_size, '\0', pipe_read_fd);
        // printf("Output = '%s'", output);
        
        // Free resources
        close(pipe_read);

        int status;
        waitpid(pid, &status, 0);

        return output;
    } else {
        // Child: Run process
        // Change stdout to pipe_out
        dup2(pipe_write, STDOUT_FILENO);

        // Close files
        close(pipe_read);
        close(pipe_write);

        // Run the command
        // printf("Running '%s'\n", command);
        execl("/bin/sh", "sh", "-c", command, NULL);
    }

    // Close files
    close(pipe_read);
    close(pipe_write);
    return "Output";
}

char *parallelgetoutput(int count, const char **argv_base) {
    return "Output";
}