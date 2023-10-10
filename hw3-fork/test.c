#include <stdio.h>
#include <stdlib.h>

char *getoutput(const char *command);

char *parallelgetoutput(int count, const char **argv_base);

int main() {
    // Test getoutput
    printf("Hi!\n");
    printf("Text: [[[%s]]]\n", getoutput("echo 1 2 3; sleep 2; echo 5 5"));
    printf("Bye!\n");

    char *output1 = getoutput("echo Hello There; ls .");
    printf("%s\n", output1);
    free(output1);

    // const char *argv_base[] = {
    //     "/bin/echo", "Hello there", NULL
    // };
    // char *output2 = parallelgetoutput(1, argv_base);
    // printf("%s\n", output2);
}