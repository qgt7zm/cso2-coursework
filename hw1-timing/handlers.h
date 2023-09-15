#include <stdio.h>
#include <unistd.h>

#include <signal.h>

#include "globals.h"
#include "timer.h"

#ifndef HANDLERS_H
#define HANDLERS_H

// Signal Handler Functions
static void handleSignalFromSelf(int signum) {
	if (signum == SIGUSR1) {
		stopTimer(); // Stop the timer after the handler starts
		signalsReceived += 1;
		// printf("Process %d received a signal!\n", getpid());
	}
}

static void handleSignalFromOther(int signum) {
	if (signum == SIGUSR1) {
		// Send back signal to first program
		kill(otherPid, SIGUSR2);
		signalsReceived += 1;
		// printf("Process %d received a signal!\n", getpid());
	} else if (signum == SIGUSR2) {
		// Stop timer
		waitForSignal = 0;
		signalsReceived += 1;
		stopTimer(); // Stop the timer after the handler starts
	}
}

static void setSignalHandlerForSelf() {
	struct sigaction action;
	action.sa_handler = handleSignalFromSelf;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	sigaction(SIGUSR1, &action, NULL);
	signalsReceived = 0;
}

static void setSignalHandlerForOther() {
	struct sigaction action;
	action.sa_handler = handleSignalFromOther;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	signalsReceived = 0;
}

// PID Functions
void printOwnPid() {
	pid_t pid = getpid();
	printf("Process ID = %d\n", pid);
}

pid_t askForPid() {
	printf("Enter other process ID: ");

	int size = 64;
	char input[size];
	fgets(input, size, stdin);

	sscanf(input, "%d\n", &otherPid);
	return otherPid;
}

void waitForInterrupt() {
	int size = 64;
	char input[size];
	do {
		printf("Waiting for keyboard interrupt... ");
	} while (fgets(input, size, stdin));
}

#endif