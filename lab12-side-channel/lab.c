#include "lab.h"

#include <string.h>     // for memset
#include <stdio.h>      // for printf

// Constants
#define NUM_LETTERS 26
#define FIRST_LETTER 'a'
#define NUM_TRIALS 10
#define MAX_RETRY 5

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
    memset(buffer, FIRST_LETTER, length);
    int result;
    for (int i = 0; i < 10; i++) {
        measure_once(&result, buffer, check_passphrase);
    }

    memset(buffer, '\0', length);

    // Time each letter and find the max
    for (int i = 0; i < length; i++) {
        long maxTiming = 0;
        int maxIdx = -1;
        for (int j = 0; j < NUM_LETTERS; j++) {
            buffer[i] = FIRST_LETTER + j;
            // Find the total after multiple trials
            long timing = 0;
            for (int k = 0; k < NUM_TRIALS; k++) {
                timing += measure_once(&result, buffer, check_passphrase);
                // printf("'%s' took %ld cycles\n", buffer, timing / NUM_TRIALS);
            }

            // Update max
            if (timing > maxTiming) {
                maxTiming = timing;
                maxIdx = j;
            }
        }
        buffer[i] = FIRST_LETTER + maxIdx;
        printf("'%s' took %ld cycles\n", buffer, maxTiming / NUM_TRIALS);
    }

    // Check passphrase
    measure_once(&result, buffer, check_passphrase);
    if (result == 1) {
        printf("Found correct passphrase\n");
        return;
    } else {
        printf("Found incorrect passphrase\n");
        // TODO retry
    }
}
