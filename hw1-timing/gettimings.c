#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

// User Headers
#include "funcs.h"
#include "handlers.h"
#include "timer.h"

// Globals
#define NUM_TRIALS 1000

long long timeStart = 0l;
long long timeStop = 0l;

long long trialTimes[NUM_TRIALS];

// Timer Functions
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

// Objective: Estimate the duration of different functions
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/systiming.html
int main(int argc, char *argv[]) {
	createSignalHandler();

	// Parse CL Args
	int funcChoice;
	pid_t otherPid;

	if (argc > 1) {
		funcChoice = atoi(argv[1]);
	} else {
		return 1;
	}

	// Warm Up
	for (int i = 0; i < NUM_TRIALS * 10; i++) {
		startTimer();
		stopTimer();
	}

	// Record Overhead Time
	long double overheadTime = 0l;
	for (int i = 0; i < NUM_TRIALS; i++) {
		startTimer();
		stopTimer();
		recordTrial(i);
	}
	overheadTime = getAvgTime();

	// TODO function pointers?
	switch (funcChoice) {
		case 1:
			// TODO fix negative times
			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				emptyFunc();
				stopTimer();
				recordTrial(i);
			}
			break;
		case 2:
			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				runSyscall();
				stopTimer();
				recordTrial(i);
			}
			break;
		case 3:
			// TODO fix super long times
			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				runShellCmd();
				stopTimer();
				recordTrial(i);
			}
			
			break;
		case 4:
			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				signalCurrentProcess();
				recordTrial(i);
			}
			break;
		case 5:
			// TODO fix negative times
			printOwnPid();
			otherPid = askForPid();

			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				signalOtherProcess(otherPid);
				stopTimer();
				recordTrial(i);
			}
			break;
		case -1:
			printOwnPid();
			otherPid = askForPid();
			signalOtherProcess(otherPid);
			
			waitForInterrupt();
			break;
		default:
			return 1;
			break;
	}

	if (funcChoice == -1) {
		return 0;
	}

	// Calculate Final Result
	long long totalTime = getTotalTime();
	long double avgTime = totalTime / (double) NUM_TRIALS;

	// Log Results
	FILE *file = fopen("timings.txt","w");
	fprintf(file, "<< Test Results >>\n");
	fprintf(file, "Program Argument = %d\n", funcChoice);
	fprintf(file, "\n");

	fprintf(file, "Overhead time = %.0Lf ns\n", overheadTime);
	fprintf(file, "\n");

	fprintf(file, "Total time with overhead = %lld ns\n", totalTime);
	fprintf(file, "Number of trials = %d\n", NUM_TRIALS);
	fprintf(file, "\n");

	fprintf(file, "Avg time w/ overhead = Total time / Num trials\n");
	fprintf(file, "Avg time with overhead = %.0Lf ns\n", avgTime);
	fprintf(file, "\n");
	
	avgTime -= overheadTime;
	fprintf(file, "Avg time w/o overhead = Avg time w/ overhead - Overhead\n");
	fprintf(file, "Avg time = %.0Lf ns\n", avgTime);
	fprintf(file, "Avg time = %Lf ms\n", avgTime / 1000000.0l);
	fprintf(file, "Avg time = %Lf sec\n", avgTime / 100000000.0l);

	return 0;
}