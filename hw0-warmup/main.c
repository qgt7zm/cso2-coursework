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

void test_sep(char *expected, int argc, char *argv[]) {
	char *result = get_sep(argc, argv);
	if (strcmp(expected, result) == 0) {
		printf("Test passed\n");
	} else {
		printf("Test failed; expected '%s'\n", expected);
	}
	free(result);
}

void test() {
	// Test the getSep() function
	char* sep0[] = {"split"};
	test_sep(" \t", 1, sep0);

	char* sep1[] = {"split", "abc"};
	test_sep("abc", 2, sep1);

	char* sep2[] = {"split", "ab", "c"};
	test_sep("abc", 3, sep2);

	char* sep3[] = {"split", "a", "b", "c"};
	test_sep("abc", 4, sep3);
}	

int main(int argc, char *argv[]) {
	// test();

    const char *input = "Computer Systems and Organization 2";
	char *sep = get_sep(argc, argv);
    int size;

	printf("Input: %s\n", input);
	printf("Sep: '%s'\n", sep);

    char **result = string_split(input, sep, &size);

	printf("Size: %d\n", size);
    printf("Result: ");
	for (int i = 0; i < size; i++) {
		printf("[%s]", result[i]);
	}
	printf("\n");

	free(sep);
	free(result);
    return 0;
}