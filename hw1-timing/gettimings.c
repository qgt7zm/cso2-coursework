#include <stdio.h>
#include <stdlib.h>

// User Headers
#include "funcs.h"
#include "handlers.h"
#include "timer.h"

// Globals
int funcChoice;
long double overheadTime;

// Helper Functions
void displayResults() {
	// Calculate Final Result
	long long totalTime = getTotalTime();
	long double avgTime = totalTime / (double) NUM_TRIALS;

	// Log Results
	FILE *file = fopen("timings.txt","w");
	fprintf(file, "<< Test Results >>\n");
	fprintf(file, "Program Argument = %d\n", funcChoice);
	fprintf(file, "Number of trials = %d\n", NUM_TRIALS);
	fprintf(file, "\n");

	fprintf(file, "Average overhead time = %.0Lf ns\n", overheadTime);
	fprintf(file, "\n");

	fprintf(file, "Total time with overhead = %lld ns\n", totalTime);
	fprintf(file, "\n");

	fprintf(file, "Avg time w/ overhead = Total time / Num trials\n");
	fprintf(file, "Avg time with overhead = %.0Lf ns\n", avgTime);
	fprintf(file, "\n");
	
	avgTime -= overheadTime;
	fprintf(file, "Avg time w/o overhead = Avg time w/ overhead - Overhead\n");
	fprintf(file, "Avg time = %.0Lf ns\n", avgTime);
	fprintf(file, "Avg time = %Lf ms\n", avgTime / 1000000.0l);
	fprintf(file, "Avg time = %Lf sec\n", avgTime / 100000000.0l);
}

// Objective: Estimate the duration of different functions
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/systiming.html
int main(int argc, char *argv[]) {
	// Parse CL Args
	pid_t otherPid;

	if (argc > 1) {
		funcChoice = atoi(argv[1]);
	} else {
		printf("Usage: gettimings <func>\n");
		return 1;
	}

	// Warm Up
	for (int i = 0; i < NUM_TRIALS * 10; i++) {
		startTimer();
		stopTimer();
	}

	// Record Overhead Time
	for (int i = 0; i < NUM_TRIALS; i++) {
		startTimer();
		stopTimer();
		recordTrial(i);
	}
	overheadTime = getAvgTime();

	// Time the Function
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
			setSignalHandler();
			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				signalCurrentProcess();
				// Don't need wait for signal since sending to same process
				recordTrial(i);
			}
			printf("Received %d/%d signals.\n", signalsReceived, NUM_TRIALS);
			break;
		case 5:
			setSignalHandler();
			setSignalBlocker();
			printOwnPid();
			otherPid = askForPid();

			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();

				blockSignals();
				signalOtherProcess(otherPid);
				printf("Signaled other process\n");
				waitForSignalRecieved();
				unblockSignals();

				stopTimer();
				// TODO send back signal
				signalsReceived += 1;
				recordTrial(i);
			}
			printf("Received %d/%d signals.\n", signalsReceived, NUM_TRIALS);
			break;
		case -1:
			setSignalHandler();
			setSignalBlocker();
			printOwnPid();
			otherPid = askForPid();

			// while (1) {
			// 	blockSignals();
			// 	signalOtherProcess(otherPid);
			// 	waitForSignalRecieved();
			// 	unblockSignals();

			// 	signalsReceived += 1;
			// }
			
			waitForInterrupt();
			printf("Received %d/%d signals.\n", signalsReceived, NUM_TRIALS);
			break;
		default:
			printf("Argument 'num' must be an integer 1-5 or -1.\n");
			return 1;
			break;
	}

	if (funcChoice != -1) {
		displayResults();
	}
	printf("Program completed.\n");
	return 0;
}