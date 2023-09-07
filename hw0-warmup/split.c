#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

char *createString(int length) {
	return calloc(sizeof(char), length + 1);
}

char **createStringArray(int length) {
	return calloc(sizeof(char*), length);
}

char *get_sep(int argc, char *argv[]) {
	const int minIdx = 1;
	const int defaultSize = 2 + 1;

	// Return whitespace by default
	if (argc == minIdx) {
		char *sep = createString(defaultSize);
		strcpy(sep, " \t");
		return sep;
	}

	// Create string
	int sepLen = 0;
	int curIdx = sepLen;
	char *sep = createString(sepLen + 1);

	for (int i = minIdx; i < argc; i++) {
		// Grow string
		sepLen += strlen(argv[i]);
		sep = realloc(sep, sizeof(char) * sepLen + 1);
		
		// Concatenate next arg
		strcpy(sep + curIdx, argv[i]);
		curIdx = sepLen;
	}
	return sep;
}

/*
 * Rules:
 * - One empty token allowed at beginning/end
 * - No empty tokens allowed in middle
 * - No double empty tokens allowed anywhere
 */
char **string_split(const char *input, const char *sep, int *num_words) {
	*num_words = 0;
	char **result = calloc(sizeof(char *), *num_words + 1);

	int startIdx = 0;
	int endIdx = 0;

	int inputLen = strlen(input);
	while (endIdx < inputLen) {
		int tokenLen = strcspn(input + startIdx, sep);

		if (tokenLen == 0) { // Found empty token
			if (*num_words == 0) {
				// Allow empty first token
			} else if (startIdx + tokenLen >= inputLen) {
				// Allow last token empty
			} else {
				// Skip empty tokens in middle
				startIdx += 1;
				endIdx += 1;
				continue;
			}
		}
		
		// Append next token
		char *token = createString(tokenLen + 1);
		strncpy(token, input + startIdx, tokenLen);
		result[*num_words] = token;

		// Resize array
		*num_words += 1;
		result = realloc(result, sizeof(char*) * (*num_words + 1));

		// Shift to next word
		endIdx = startIdx + tokenLen;
		startIdx = endIdx + 1;
	}
	
	return result;
}
