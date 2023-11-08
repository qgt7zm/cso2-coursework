#include <math.h> // log, exp
#include <stdlib.h> // free, realloc

#include "openmpstarter.h"

// Approach: Even Split + Atomic Reduction
double geomean(unsigned char *s, size_t n) {
    double answer = 0;
    // separate for loop into smaller parallel loops
    # pragma omp parallel for
    for(int i=0; i<n; i+=1) {
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
    run_time_trial(s, n, "Even split + atomic reduction");
}
