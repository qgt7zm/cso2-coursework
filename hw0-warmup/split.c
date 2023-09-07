#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

char **string_split(const char *input, const char *sep, int *num_words) {
	*num_words = 0;
	char **result = (char **) calloc(sizeof(char *), *num_words + 1);

	int startIdx = 0;
	int endIdx = 0;

	while (endIdx < strlen(input)) {
		// printf("Start: %d\n", startIdx);
		// printf("Split: %d\n", splitIdx);
		// printf("Remaining: '%s'\n", input + startIdx);
		// Find end of token
		endIdx = startIdx + strcspn(input + startIdx, sep);
		
		// Get next token
		int len = endIdx - startIdx;
		char token[len + 1];
		strncpy(token, input + startIdx, len + 1);
		token[len] = '\0';

		printf("Token: '%s'\n", token);
		printf("Len: %d\n", len);

		startIdx = endIdx + 1;

		// Append token
		result[*num_words] = "foo";

		// Resize
		*num_words += 1;
		result = (char **) realloc(result, *num_words + 1);
	}

	return result;
}