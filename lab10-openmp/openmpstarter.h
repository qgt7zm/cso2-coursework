#include <stdio.h> // fopen, fread, fclose, printf, fseek, ftell
#include <stdlib.h> // free, realloc
#include <time.h> // struct timespec, clock_gettime, CLOCK_REALTIME
#include <errno.h>

#ifndef OPENMPSTARTER_H
#define OPENMPSTARTER_H

// Computes the geometric mean of a set of values.
// You should use OpenMP to make faster versions of this.
// Keep the underlying sum-of-logs approach.
double geomean(unsigned char *s, size_t n);

// Nanoseconds that have elapsed since 1970-01-01 00:00:00 UTC
long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

// Read the files specified in argv and sets the length
char *read_file(int argc, char *argv[], size_t *n) {
    char *s = NULL;
    *n = 0;
    for(int i = 1; i < argc; i++) {
        // add argument i's file contents (or string value) to s
        FILE *f = fopen(argv[i], "rb");
        if (f) { // was a file; read it
            fseek(f, 0, SEEK_END); // go to end of file
            size_t size = ftell(f); // find out how many bytes in that was
            fseek(f, 0, SEEK_SET); // go back to beginning
            s = (char *) realloc(s, *n + size); // make room
            int ret = fread(s + *n, 1, size, f); // append this file on end of others
            fclose(f);
            *n += size; // not new size
        } else { // not a file; treat as a string
            errno = 0; // clear the read error
        }
    }
    return s;
}

void run_time_trial(char *s, size_t n, char *strategy) {
    // Time the function
    long long t0 = nsecs();
    double answer = geomean((unsigned char*) s, n);
    long long t1 = nsecs();
    free(s);

    // Print results
    long long time = t1 - t0;

    printf("Strategy: %s\n", strategy);
    printf("Time: %lld ms, ", time);
    printf("Characters: %zd, ", n);
    printf("Result: %g\n", answer);
    
    // printf("%lld ns to process %zd characters: %g\n\n", t1 - t0, n, answer);
    puts("");
}

#endif