#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

// TODO too many blank strings if adjacent tokens
char **string_split(const char *input, const char *sep, int *num_words) {
	*num_words = 0;
	char **result = (char **) calloc(sizeof(char *), *num_words + 1);

	int startIdx = 0;
	int endIdx = 0;

	while (endIdx < strlen(input)) {
		int span = strcspn(input + startIdx, sep);

		// if (span == 0) {
		// 	// Only found token
		// 	startIdx += 1;
		// 	endIdx += 1;
		// 	continue;
		// }

		// Find end of token
		endIdx = startIdx + span;
		
		// Get next token
		int len = endIdx - startIdx;
		char *token = calloc(sizeof(char), len + 1);
		strncpy(token, input + startIdx, len);
		token[len] = '\0'; // Append null terminator

		// Update indices
		startIdx = endIdx + 1;

		// Append token
		result[*num_words] = token;
		// printf("[%s]", result[*num_words]);

		// Resize array
		*num_words += 1;
		result = (char **) realloc(result, *num_words + 1);
	}

	return result;
}

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
