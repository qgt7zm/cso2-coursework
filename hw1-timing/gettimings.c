#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "funcs.h"
#include "handlers.h"
#include "timer.h"

// Globals
int funcChoice;
char* funcName;
long double overheadTime;

// Helper Functions
void getFuncName() {
	switch(funcChoice) {
		case 1:
			funcName = "Empty function";
			break;
		case 2:
			funcName = "System call";
			break;
		case 3:
			funcName = "Shell command";
			break;
		case 4:
			funcName = "Send signal to self";
			break;
		case 5:
			funcName = "Send signal to other program";
			break;
		default:
			funcName = "";
			break;
	}
}

void displayResults() {
	getFuncName();
	
	// Calculate Final Result
	long long totalTime = getTotalTime();
	long double avgTime = totalTime / (double) NUM_TRIALS;

	// Log Results
	FILE *file = fopen("timings.txt","a");

	fprintf(file, "<< Test Results >>\n");
	fprintf(file, "Program argument = %d\n", funcChoice);
	fprintf(file, "Function tested = %s\n", funcName);
	fprintf(file, "Number of trials = %d\n", NUM_TRIALS);
	fprintf(file, "\n");

	fprintf(file, "Average overhead time = %.0Lf ns\n", overheadTime);
	fprintf(file, "\n");

	fprintf(file, "Total time with overhead = %lld ns\n", totalTime);
	fprintf(file, "Avg time w/ overhead = Total time / Num trials\n");
	fprintf(file, "Avg time with overhead = %.0Lf ns\n", avgTime);
	fprintf(file, "\n");
	
	long double avgTimeNoOverhead = avgTime - overheadTime;
	fprintf(file, "Avg time w/o overhead = Avg time w/ overhead - Overhead\n");
	fprintf(file, "Avg time = %.0Lf ns\n", avgTimeNoOverhead);
	fprintf(file, "Avg time = %Lf ms\n", avgTimeNoOverhead / 1000000.0l);
	fprintf(file, "Avg time = %Lf sec\n", avgTimeNoOverhead / 100000000.0l);
	fprintf(file, "\n");
}

// Objective: Estimate the duration of different functions
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/systiming.html
int main(int argc, char *argv[]) {
	// Parse CL Args
	if (argc > 1) {
		funcChoice = atoi(argv[1]);
	} else {
		printf("Usage: gettimings <func>\n");
		return 1;
	}

	// Warm up the CPU
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
			setSignalHandlerForSelf();
			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();
				signalCurrentProcess();
				// Don't need wait for signal since sending to same process
				recordTrial(i);
			}
			printf("Received %d/%d signals.\n", signalsReceived, NUM_TRIALS);
			break;
		case 5:
			setSignalHandlerForOther();
			printOwnPid();
			askForPid();

			for (int i = 0; i < NUM_TRIALS; i++) {
				startTimer();

				waitForSignal = 1;
				signalOtherProcess();
				// printf("Sent signal to other process\n");

				// Wait for signal
				while (waitForSignal) {
					usleep(10);
				}
				// printf("Signal received by other process\n");

				recordTrial(i);
			}
			printf("Received %d/%d signals.\n", signalsReceived, NUM_TRIALS);
			break;
		case -1:
			setSignalHandlerForOther();
			printOwnPid();
			askForPid();
			
			waitForInterrupt();
			printf("Received %d/%d signals.\n", signalsReceived, NUM_TRIALS);
			break;
		default:
			printf("Argument 'num' must be an integer 1-5 or -1.\n");
			return 1;
	}

	if (funcChoice != -1) {
		displayResults();
	}
	printf("Program completed.\n");
	return 0;
}