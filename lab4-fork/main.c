#include <stdio.h>

#include "mysystem.h"

int main() {
    char *command = "ls -l";
    int code = my_system(command);
    printf("Command exited with code %d.\n", code);
}