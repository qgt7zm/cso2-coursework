#include <stddef.h>
#include <stdio.h> // fopen, fread, fclose, printf, fseek, ftell
#include <math.h> // log, exp
#include <stdlib.h> // free, realloc

#include "openmpstarter.h"

// Approach: Task Queue + Many-to-few Reduction
double geomean(unsigned char *s, size_t n) {
    double answer = 0;
    int j = 0; // shared counter

    // run the while loop in parallel
    # pragma omp parallel
    {
        double local_answer = 0; // keep a local result
        while (1) {
            int i;

	    // run the operation atomically
            # pragma omp atomic capture
	    i = j++;
	    if (i >= n) break;

            # pragma omp nowait
            if (s[i] > 0) {
	        // run the operation atomically
	        # pragma omp atomic update
	        local_answer += log(s[i]) / n;
	    }	
	}

	// run the operation atomically
        # pragma omp atomic update
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
    run_time_trial(s, n, "Task queue + many-to-few");
}
