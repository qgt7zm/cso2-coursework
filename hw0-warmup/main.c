#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

int main(int argc, char *argv[]) {
	// test();
	// exit(0);

	char *sep = get_sep(argc, argv);
	// printf("Sep: '%s'\n", sep);

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

		// break;
	}

	free(sep);
    return 0;
}
