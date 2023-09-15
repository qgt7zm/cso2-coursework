#define _XOPEN_SOURCE 600
#include <time.h>

#ifndef TIMER_H
#define TIMER_H

// Global Variables
#define NUM_TRIALS 1000

static long long timeStart = 0l;
static long long timeStop = 0l;
static long long trialTimes[NUM_TRIALS];

// Timer Functions
long long getnsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

void startTimer() {
	timeStart = getnsecs();
}

void stopTimer() {
	timeStop = getnsecs();
}

void recordTrial(int currTrial) {
	long long trialTime = timeStop - timeStart;
	trialTimes[currTrial] = trialTime;
}

long long getTotalTime() {
	long long totalTime = 0l;
	for (int i = 0; i < NUM_TRIALS; i++) {
		totalTime += trialTimes[i];
	}
	return totalTime;
}

long double getAvgTime() {
	return getTotalTime() / (double) NUM_TRIALS;
}

#endif