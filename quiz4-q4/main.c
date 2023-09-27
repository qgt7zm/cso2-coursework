#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global;

int main() {
    global = 0;
    printf("Global = %p\n", &global);

    pid_t p = fork();
    if (p == 0) {
        global = 3;
        printf("Parent Global = %p\n", &global); fflush(stdout);
    } else {
        global = 4;
        printf("Child Global = %p\n", &global); fflush(stdout);
    }
    return 0;
}