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

        // Read lines
        size_t line_size = 1024;
        char *line = calloc(line_size + 1, sizeof(char));
        int chars_read = 0;

        // Store entire output
        char *output = calloc(line_size + 1, sizeof(char));
        size_t output_size = (int) line_size;
        int output_chars = 0;

        while ((chars_read = getdelim(&line, &line_size, '\n', pipe_read_fd))) {
            // printf("Line = '%s'", line);
            if (chars_read == -1) break; // End of file

            // Append lines to output
            output_chars += chars_read;
            // printf("Size = '%d'\n", output_chars);

            // Reallocate output
            if (output_chars >= output_size) {
                output_size += line_size;
                output = realloc(output, output_size + 1);
            }

            strncat(output, line, output_size);
            // printf("Output = '%s'\n", output);
        }
        
        // Free resources
        free(line);
        close(pipe_read);
        
        int status;
        waitpid(pid, &status, 0);

        return output;
    } else {
        // Child
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