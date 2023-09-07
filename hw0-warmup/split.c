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

		// Resize array
		*num_words += 1;
		result = (char **) realloc(result, *num_words + 1);
	}

	return result;
}
