#include <stdio.h>

#include "my_system.h"

int main() {
    // Example 1
    int a1 = my_system("sleep 1; echo hi");
    int a2 = my_system("echo bye");
    int a3 = my_system("flibbertigibbet 23");
    printf("%d %d %d\n", a1, a2, a3);
    
    // Example 2
    int a4 = my_system("echo -n 'type something: ';"
        " read got;"
        " echo Thanks for typing \\\"\"$got\"\\\"");
    printf("Code = %d\n", a4);

    char *command = "ls";
    int a5 = my_system(command);
    printf("Code = %d\n", a5);
}