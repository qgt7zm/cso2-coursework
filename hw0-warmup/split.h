#ifndef SPLIT_H
#define SPLIT_H

#define INPUT_SIZE 4096

char **string_split(const char *input, const char *sep, int *num_words);

char *get_sep(int argc, char *argv[]);

#endif
