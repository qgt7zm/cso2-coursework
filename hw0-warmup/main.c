#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

char *get_sep(int argc, char *argv[]) {
	const int minIdx = 1;

	// Return whitespace by default
	if (argc == minIdx) {
		char *sep = calloc(sizeof(char), 2 + 1);
		strcpy(sep, " \t");
		return sep;
	}

	// Create string
	int sepLen = 0;
	int curIdx = sepLen;
	char *sep = calloc(sizeof(char), sepLen + 1);

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
	char *sep = get_sep(argc, argv);

	char buffer[INPUT_SIZE + 1];
	while(1) {
		// Get user input
		char *input = fgets(buffer, INPUT_SIZE, stdin);

		if (!input) break;

		// Prune newline
		int len = strlen(input);
		if (input[len - 1] == '\n') input[len - 1] = '\0';

		if (strcmp(input, ".") == 0) {
			// Stop if period
			break;
		}

		int size;
		char **result = string_split(input, sep, &size);

		for (int i = 0; i < size; i++) {
			printf("[%s]", result[i]);
		}
		printf("\n");
		free(result);
	}

	free(sep);
    return 0;
}
