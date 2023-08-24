#include <stdio.h>

#include "split.h"

int main(int argc, char *argv[]) {
    const char *input = "Computer Systems and Organization 2";
    const char *sep = "CSO2";
    int size;
    char **result = string_split(input, sep, &size);
    printf("%s %s %d\n", input, sep, size);
    printf("%s\n", result[0]);
    return 0;
}