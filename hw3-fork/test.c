#include <stdio.h>

char *getoutput(const char *command);

char *parallelgetoutput(int count, const char **argv_base);

int main() {
    char *output1 = getoutput("ls .");
    printf("%s\n", output1);

    const char *argv_base[] = {
        "/bin/echo", "Hello there", NULL
    };
    char *output2 = parallelgetoutput(1, argv_base);
    printf("%s\n", output2);
}