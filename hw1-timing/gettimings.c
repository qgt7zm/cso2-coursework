#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "funcs.h"
#include "handlers.h"

long long getnsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

void getTimeDiff(long long timeStart, long long timeEnd) {
	long long timeDiff = timeEnd - timeStart;
	printf("That took %lld ns!\n", timeDiff);
	printf("That took %f ms!\n", timeDiff / 1000000.0);
	printf("That took %f sec!\n", timeDiff / 1000000000.0);
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

	long long timeStart = getnsecs();
	
	switch (funcChoice) {
		case 1:
			emptyFunc();
			break;
		case 2:
			runSyscall();
			break;
		case 3:
			runShellCmd();
			break;
		case 4:
			signalCurrentProcess();
			break;
		case 5:
			printOwnPid();
			otherPid = askForPid();
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
	getTimeDiff(timeStart, timeEnd);
	return 0;
}