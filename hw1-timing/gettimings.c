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
#define NUM_TRIALS 500

long long timeStart = 0l;
long long timeStop = 0l;

long long trialTimes[NUM_TRIALS];

// Header Functions
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

long double getAvgTime() {
	long long totalTime = 0l;
	for (int i = 0; i < NUM_TRIALS; i++) {
		totalTime += trialTimes[i];
	}
	return totalTime / (double) NUM_TRIALS;
}

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
	printf("Overhead time is %.0Lf ns\n", overheadTime);

	// TODO function pointers?
	switch (funcChoice) {
		case 1:
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
			printOwnPid();
			otherPid = askForPid();

			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				signalOtherProcess(otherPid);
				recordTrial(i);
			}
			break;
		case -1:
			printOwnPid();
			otherPid = askForPid();
			
			askForPid();
			break;
		default:
			return 1;
			break;
	}

	long double avgTime = getAvgTime() - overheadTime;
	printf("That took %.0Lf ns!\n", avgTime);
	printf("That took %Lf ms!\n", avgTime / 1000000.0l);
	printf("That took %Lf sec!\n", avgTime / 1000000000.0l);
	return 0;
}