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

    // step 2: invoke and time the geometric mean function
    long long t0 = nsecs();
    double answer = geomean((unsigned char*) s,n);
    long long t1 = nsecs();

    free(s);

    // step 3: report result
    printf("Strategy: Task queue + atomic reduction\n");
    printf("%lld ns to process %zd characters: %g\n\n", t1-t0, n, answer);
}
