#include <stddef.h>
#include <stdio.h> // fopen, fread, fclose, printf, fseek, ftell
#include <math.h> // log, exp
#include <stdlib.h> // free, realloc

#include "openmpstarter.h"

// Approach: Task Queue + Atomic Reduction
double geomean(unsigned char *s, size_t n) {
    double answer = 0;
    int j = 0; // shared counter

    // run the while loop in parallel
    # pragma omp parallel
    while (1) {
        int i;
	// run the operation atomically
        # pragma omp atomic capture
	i = j++;
	if (i >= n) break;

        if (s[i] > 0) {
	    // run the operation atomically
	    # pragma omp atomic update
	    answer += log(s[i]) / n;
	}
    }
    return exp(answer);
}

int main(int argc, char *argv[]) {
    // Step 1: get the input array (the bytes in this file)
    size_t n;
    char *s = read_file(argc, argv, &n);

    // Step 2: invoke and time the geometric mean function
    // Step 3: report result
    run_time_trial(s, n, "Task queue + atomic reduction");
}
