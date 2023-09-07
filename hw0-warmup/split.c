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
		// result[*num_words] = strndup(input + startIdx, len); // strdup not defined on some OS

		char *token = calloc(sizeof(char), len + 1);
		strncpy(token, input + startIdx, len);
		token[len] = '\0'; // Append null terminator
		result[*num_words] = token;

		// Resize array
		*num_words += 1;
		result = realloc(result, sizeof(char*) * (*num_words + 1));
		startIdx = endIdx + 1;
	}
	
	return result;
}
