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
long long timeStart;
long long timeStop;

// Header Functions
void startTimer() {
	timeStart = getnsecs();
}

void stopTimer() {
	timeStop = getnsecs();
}

int main(int argc, char *argv[]) {
	createSignalHandler();

	int funcChoice;
	pid_t otherPid;

	if (argc > 1) {
		funcChoice = atoi(argv[1]);
	} else {
		return 1;
	}

	switch (funcChoice) {
		case 1:
			startTimer();
			emptyFunc();
			stopTimer();
			break;
		case 2:
			startTimer();
			runSyscall();
			stopTimer();
			break;
		case 3:
			startTimer();
			runShellCmd();
			break;
		case 4:
			startTimer();
			signalCurrentProcess();
			break;
		case 5:
			printOwnPid();
			otherPid = askForPid();
			startTimer();
			signalOtherProcess(otherPid);
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

	long long timeEnd = getnsecs();

	long long timeDiff = timeEnd - timeStart;
	printf("That took %lld ns!\n", timeDiff);
	printf("That took %Lf ms!\n", timeDiff / 1000000.0l);
	printf("That took %Lf sec!\n", timeDiff / 1000000000.0l);
	return 0;
}