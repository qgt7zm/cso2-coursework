#include "lab.h"

#include <string.h>     // for memset
#include <stdio.h>      // for printf

// Constants
#define NUM_LETTERS 26
#define FIRST_LETTER 'a'

/* When complete, this function should fill in "buffer"
 * with a length-character \0-termianted string such that
 * check_passphrase(buffer) is true.
 *
 * The implementation we supplied tries the guesses of
 * 'a', 'b', and 'c' and prints out how long it takes
 * to check each of them.
 *
 * To do so, your implementation should rely on timing
 * how long check_passphrase takes, most likely by using
 * "measure_once" wrapper function.
 *
 * (Your implementation may not examine the memory in which
 *  the passphrase is stored in another way.)
 */
void find_passphrase(char *buffer, int length) {
    // Warm up the processor
    memset(buffer, 'c', length);
    int result;
    for (int i = 0; i < 10; i++) {
        measure_once(&result, buffer, check_passphrase);
    }

    memset(buffer, '\0', length);
    result = 0;

    for (int i = 0; i < NUM_LETTERS; i++) {
        char next_char = FIRST_LETTER + i;
        buffer[0] = next_char;
        long time_for_a = measure_once(&result, buffer, check_passphrase);
        printf("'%c' took %ld cycles\n", next_char, time_for_a);
        if (result == 1) {
            // Found correct passphrase, done
            return;
        }
    }
}
