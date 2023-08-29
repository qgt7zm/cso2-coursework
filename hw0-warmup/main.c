#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

// TODO this should be on stack
char *getSep(int argc, char *argv[]) {
	const int minIdx = 1;

	if (argc == minIdx) {
		char *sep = calloc(sizeof(char), 2 + 1);
		strcpy(sep, " \t");
		return sep;
	}

	// Create string
	int sepLen = 0;
	int curIdx = sepLen;
	char *sep = (char *) calloc(sizeof(char), sepLen + 1);

	for (int i = minIdx; i < argc; i++) {
		// Grow string
		sepLen += strlen(argv[i]);
		sep = (char *) realloc(sep, sepLen + 1);
		// Concatenate next arg
		strcpy(sep + curIdx, argv[i]);
		curIdx = sepLen;
	}
	return sep;
}

int main(int argc, char *argv[]) {
    const char *input = "Computer Systems and Organization 2";
    char *sep = getSep(argc, argv);
    int size;
    char **result = string_split(input, sep, &size);

    printf("Input: %s\n", input);
	printf("Sep: %s\n", sep);
	printf("Size: %d\n", size);
    printf("Result: %s\n", result[0]);

	free(sep);
    return 0;
}