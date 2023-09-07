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

void test_sep(char expected[], int argc, char *argv[]) {
	char *result = get_sep(argc, argv);
	if (strcmp(expected, result) == 0) {
		printf("Test passed\n");
	} else {
		printf("Test failed; expected '%s' but found '%s'\n", expected, result);
	}
	free(result);
}

void test_split(char *expected_results[], int expected_size, const char *input, const char *sep) {
	int size;
	char **results = string_split(input, sep, &size);

	int passed = 1;
	if (expected_size != size) {
		passed = 0;
		printf("Test failed; expected %d tokens but found %d\n", expected_size, size);
	}
	for (int i = 0; i < expected_size && i < size; i++) {
		if (strcmp(expected_results[i], results[i]) != 0) {
			printf("> Expected token '%s' but found '%s' at position %d\n",
				expected_results[i], results[i], i);
			passed = 0;
		}
	}
	if (passed) {
		printf("Test passed\n");
	}

	free(results);
}

int main() {
	/* Test the get_sep() function */
	char *sep0[] = {"split"};
	test_sep(" \t", 1, sep0);

	char *sep1[] = {"split", "abc"};
	test_sep("abc", 2, sep1);

	char *sep2[] = {"split", "ab", "c"};
	test_sep("abc", 3, sep2);

	char *sep3[] = {"split", "a", "b", "c"};
	test_sep("abc", 4, sep3);

	/* Test the string_split() function */
	// Failing
	char *results1[] = {"Computer", "Systems", "and", "Organization", "2"};
	test_split(results1, 5, "Computer Systems and Organization 2", " ");

	// Examples 2.1
	char *results2[] = {"foo"};
	test_split(results2, 1, "foo", ":");

	char *results3[] = {"foo", "bar", "quux"};
	test_split(results3, 3, "foo:bar!quux", ":!");
	test_split(results3, 3, "foo:bar!quux", "!:");

	// Failing
	char *results4[] = {"", "foo", "bar", "quux", ""};
	test_split(results4, 5, ":foo!:bar::quux!", ":!");

	// Examples 2.2
	// Failing
	char *results5[] = {"foo", "bar", "baz"};
	test_split(results5, 3, "foo\t bar   baz", " \t");

	char *results6[] = {"quux-no-space", "quux-with", "space", "!"};
	test_split(results6, 4, "quux-no-space quux-with space !", " \t");

	char *results7[] = {"", "indented"};
	test_split(results7, 2, "\tindented", " \t");

	// Failing
	char *results8[] = {"foo", "bar", "baz", ""};
	test_split(results8, 4, "fooXXXXbarZXYXYXZbazYYYYY", "XYZ");

	// Failing
	char *results9[] = {"", ""};
	test_split(results9, 2, "XXXXXXXXXXXXX", "XYZ");
	
	char *results10[] = {"", ".", ""};
	test_split(results10, 3, "X.X", "XYZ");
}
