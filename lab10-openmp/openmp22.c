#include <stdio.h> // fopen, fread, fclose, printf, fseek, ftell
#include <math.h> // log, exp
#include <stdlib.h> // free, realloc
#include <time.h> // struct timespec, clock_gettime, CLOCK_REALTIME
#include <errno.h>

// computes the geometric mean of a set of values.
// Task Queue + Many-to-few Reduction approach
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

/// nanoseconds that have elapsed since 1970-01-01 00:00:00 UTC
long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

/// reads arguments and invokes geomean; should not require editing
int main(int argc, char *argv[]) {
    // step 1: get the input array (the bytes in this file)
    char *s = NULL;
    size_t n = 0;
    for(int i=1; i<argc; i+=1) {
        // add argument i's file contents (or string value) to s
        FILE *f = fopen(argv[i], "rb");
        if (f) { // was a file; read it
            fseek(f, 0, SEEK_END); // go to end of file
            size_t size = ftell(f); // find out how many bytes in that was
            fseek(f, 0, SEEK_SET); // go back to beginning
            s = realloc(s, n+size); // make room
            int ret = fread(s+n, 1, size, f); // append this file on end of others
            fclose(f);
            n += size; // not new size
        } else { // not a file; treat as a string
            errno = 0; // clear the read error
        }
    }

    // step 2: invoke and time the geometric mean function
    long long t0 = nsecs();
    double answer = geomean((unsigned char*) s,n);
    long long t1 = nsecs();

    free(s);

    // step 3: report result
    printf("Strategy: Task queue + many-to-few reduction\n");
    printf("%lld ns to process %zd characters: %g\n\n", t1-t0, n, answer);
}
