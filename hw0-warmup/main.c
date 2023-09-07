#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

void pruneNewLine(char *str) {
	int len = strlen(str);
	if (str[len - 1] == '\n') str[len - 1] = '\0';
}

// Objective: Create a program that splits strings along a given delineator
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/warmup.html
int main(int argc, char *argv[]) {
	char *sep = get_sep(argc, argv);

	char buffer[INPUT_SIZE + 1];
	while(1) {
		// Get user input
		char *input = fgets(buffer, INPUT_SIZE, stdin);
		if (!input) break;

		pruneNewLine(input);
		if (strcmp(input, ".") == 0) break; // Stop if period

		// Display results
		int size;
		char **result = string_split(input, sep, &size);

		for (int i = 0; i < size; i++) {
			printf("[%s]", result[i]);
			free(result[i]); // Free results while printing
		}
		printf("\n");

		free(result);
	}

	free(sep);
    return 0;
}
