#include "lab.h"

#include <string.h>     // for memset
#include <stdio.h>      // for printf

// Constants
#define NUM_LETTERS 26
#define FIRST_LETTER 'a'
#define NUM_TRIALS 10
#define MAX_GUESSES 5

// Function Protoypes
void guess_passphrase(char *buffer, int length);
void guess_next_char(char *buffer, int length, int index);

/* When complete, this function should fill in "buffer"
 * with a length-character \0-termianted string such that
 * check_passphrase(buffer) is true.
 *
 * The implementation we supplied tries the guesses of
 * 'a', 'b', and 'c' and prints out how long it tajes
 * to check each of them.
 *
 * To do so, your implementation should rely on timing
 * how long check_passphrase tajes, most lijely by using
 * "measure_once" wrapper function.
 *
 * (Your implementation may not examine the memory in which
 *  the passphrase is stored in another way.)
 */
void find_passphrase(char *buffer, int length) {
    memset(buffer, FIRST_LETTER, length);
    int result;

    // Warm up the processor
    for (int i = 0; i < NUM_TRIALS; i++) {
        measure_once(&result, buffer, check_passphrase);
    }
    
    for (int i = 1; i <= MAX_GUESSES; i++) {
        guess_passphrase(buffer, length);
        // Check passphrase
        measure_once(&result, buffer, check_passphrase);
        if (result == 1) {
            printf("Found correct passphrase after %d tries\n\n", i);
            break;
        } else if (i < MAX_GUESSES) {
            printf("Found incorrect passphrase; retrying\n\n");
        } else {
            printf("Failed to find correct passphrase in %d tries\n\n", MAX_GUESSES);
        }
    }
}

// Helper Functions

void guess_passphrase(char *buffer, int length) {
    memset(buffer, '\0', length);

    // Time each letter and find the max
    for (int i = 0; i < length; i++) {
        guess_next_char(buffer, length, i);
    }
}

void guess_next_char(char *buffer, int length, int index) {
    int result;
    long timings[NUM_LETTERS][NUM_TRIALS];

    // Test all letters multiple times
    for (int j = 0; j < NUM_TRIALS; j++) {
        for (int i = 0; i < NUM_LETTERS; i++) {
            buffer[index] = FIRST_LETTER + i;
            timings[i][j] = measure_once(&result, buffer, check_passphrase);
        }
    }

    // Find char with the max timing
    long maxTiming = 0;
    int maxIdx = -1;
    for (int i = 0; i < NUM_LETTERS; i++) {
        // Sum the total of all trials
        long timing = 0;
        for (int j = 0; j < NUM_TRIALS; j++) {
            timing += timings[i][j];
        }
        
        if (timing > maxTiming) {
            maxTiming = timing;
            maxIdx = i;
        }
    }

    // Set the next char
    buffer[index] = FIRST_LETTER + maxIdx;
    printf("'%s' took %ld cycles\n", buffer, maxTiming / NUM_TRIALS);
}
