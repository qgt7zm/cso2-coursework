#include <stddef.h>
#include <stdio.h> // fopen, fread, fclose, printf, fseek, ftell
#include <math.h> // log, exp
#include <stdlib.h> // free, realloc

#include "openmpstarter.h"

// Approach: non-parallelized
double geomean(unsigned char *s, size_t n) {
    double answer = 0;
    for(int i=0; i<n; i+=1) {
        if (s[i] > 0) answer += log(s[i]) / n;
    }
    return exp(answer);
}

// Reads arguments and invokes geomean; should not require editing
int main(int argc, char *argv[]) {
    // Step 1: get the input array (the bytes in this file)
    size_t n;
    char *s = read_file(argc, argv, &n);

    // Step 2: invoke and time the geometric mean function
    long long t0 = nsecs();
    double answer = geomean((unsigned char*) s, n);
    long long t1 = nsecs();
    free(s);

    // Step 3: report result
    printf("Strategy: Non-parallel\n");
    printf("%lld ns to process %zd characters: %g\n\n", t1 - t0, n, answer);
}
