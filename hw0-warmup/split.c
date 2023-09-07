#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

// TODO too many blank strings if adjacent tokens
char **string_split(const char *input, const char *sep, int *num_words) {
	*num_words = 0;
	char **result = calloc(sizeof(char *), *num_words + 1);

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
		
		// Append next token
		int len = endIdx - startIdx;
		result[*num_words] = strndup(input + startIdx, len);

		// Resize array
		*num_words += 1;
		result = realloc(result, sizeof(char*) * (*num_words + 1));
		startIdx = endIdx + 1;
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
