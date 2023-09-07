#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "split.h"

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
	if (expected_size == size) {
		printf("Test passed\n");
	} else {
		printf("Test failed; expected %d tokens but found %d\n", expected_size, size);
		for (int i = 0; i < expected_size && i < size; i++) {
			if (strcmp(expected_results[i], results[i]) != 0) {
				printf("> Expected token '%s' but found '%s' at position %d\n",
					expected_results[i], results[i], i);
			}
		}
	}
	free(results);
}

void test() {
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

	char *results6[] = {"quuz-no-space", "quux-with", "space", "!"};
	test_split(results6, 4, "quux-no-space quux-with space !", " \t");

	char *results7[] = {"", "indented"};
	test_split(results7, 2, "\tindented", " \t");

	// Segfault
	// char *results8[] = {"foo", "bar", "baz", ""};
	// test_split(results8, 4, "fooXXXXbarZXYXYXZbazYYYYY", "XYZ");

	// Failing
	char *results9[] = {"", ""};
	test_split(results9, 2, "XXXXXXXXXXXXX", "XYZ");
	
	char *results10[] = {"", ".", ""};
	test_split(results10, 3, "X.X", "XYZ");
}	

int main(int argc, char *argv[]) {
	// test();
	// exit(0);

	char *sep = get_sep(argc, argv);
	// printf("Sep: '%s'\n", sep);

	char buffer[INPUT_SIZE];
	while(1) {
		// Get user input
		char *input = fgets(buffer, INPUT_SIZE, stdin);
		if (!input) break;

		// Prune newline
		int len = strlen(input);
		input[len - 1] = '\0';

		if (strcmp(input, ".") == 0) {
			// Stop if period
			break;
		}

		// printf("Input: '%s'\n", input);

		int size;
		char **result = string_split(input, sep, &size);

		// printf("Size: %d\n", size);
		// printf("Result: ");
		// TODO sometimes giving garbage on first token
		for (int i = 0; i < size; i++) {
			printf("[%s]", result[i]);
		}
		printf("\n");
		free(result);
	}

	free(sep);
    return 0;
}
