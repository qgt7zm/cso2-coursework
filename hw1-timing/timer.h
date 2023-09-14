#define _XOPEN_SOURCE 700

#include <time.h>

#ifndef TIMER_H
#define TIMER_H

long long getnsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

void startTimer();
void stopTimer();

#endif