#include <math.h> // log, exp
#include <stdlib.h> // free, realloc

#include "openmpstarter.h"

// Approach: Even Split + Many-to-few Reduction
double geomean(unsigned char *s, size_t n) {
    double answer = 0;

    // run many for loops in parallel
    # pragma omp parallel
    {
        double local_answer = 0; // keep a local result
        // run a smaller for loop
        # pragma omp for nowait
        for(int i=0; i<n; i+=1) {
            if (s[i] > 0) {
                // run the operation atomically
                # pragma omp atomic update
                local_answer += log(s[i]) / n;
            }
        }
        answer += local_answer;
    }

    return exp(answer);
}

int main(int argc, char *argv[]) {
    // Step 1: get the input array (the bytes in this file)
    size_t n;
    char *s = read_file(argc, argv, &n);

    // Step 2: invoke and time the geometric mean function
    // Step 3: report result
    run_time_trial(s, n, "Even split + many-to-few");
}
