#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

/*
 * Rules:
 * - 1x "" allowed at beginning/end
 * - No "" allowed in middle
 * - No double "" allowed anywhere
 */
char **string_split(const char *input, const char *sep, int *num_words) {
	*num_words = 0;
	char **result = calloc(sizeof(char *), *num_words + 1);

	int startIdx = 0;
	int endIdx = 0;

	int inputLen = strlen(input);
	while (endIdx < inputLen) {
		int span = strcspn(input + startIdx, sep);

		if (span == 0) { // Found empty token
			if (*num_words == 0) {
				// Allow empty first token
			} else if (startIdx + span >= inputLen) {
				// Allow last token empty
			} else {
				// Skip empty tokens in middle
				startIdx += 1;
				endIdx += 1;
				continue;
			}
		}

		// Find end of token
		endIdx = startIdx + span;
		
		// Append next token
		int tokenLen = endIdx - startIdx;
		// result[*num_words] = strndup(input + startIdx, len); // strdup not defined on some OS

		char *token = calloc(sizeof(char), tokenLen + 1);
		strncpy(token, input + startIdx, tokenLen);
		token[tokenLen] = '\0'; // Append null terminator
		result[*num_words] = token;

		// Resize array
		*num_words += 1;
		result = realloc(result, sizeof(char*) * (*num_words + 1));
		startIdx = endIdx + 1;
	}
	
	return result;
}
