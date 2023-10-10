#include <stdio.h>
#include <stdlib.h>

char *getoutput(const char *command);

char *parallelgetoutput(int count, const char **argv_base);

int main() {
    // Test getoutput
    char *output1 = getoutput("echo Hello There; ls .");
    printf("%s\n", output1);
    free(output1);

    printf("Hi!\n");
    char *output2 = getoutput("echo 1 2 3; sleep 0.5; echo 5 5");
    printf("Text: [[[%s]]]\n", output2);
    printf("Bye!\n");
    free(output2);  

    // Test parallelgetoutput
    const char *argv_base[] = {
        "/bin/echo", "Hello there", NULL
    };
    char *output3 = parallelgetoutput(4, argv_base);
    printf("%s\n", output3);
    free(output3);
}