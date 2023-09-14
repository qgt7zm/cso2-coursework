#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "timer.h"

#ifndef HANDLERS_H
#define HANDLERS_H

void handleSignal(int signum) {
	if (signum == SIGUSR1) {
		stopTimer();
		// printf("Process %d received a signal!\n", getpid());
	}
}

void createSignalHandler() {
	struct sigaction action;
	action.sa_handler = handleSignal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	// Register signal action
	sigaction(SIGUSR1, &action, NULL);
}

void printOwnPid() {
	pid_t pid = getpid();
	printf("Process ID = %d\n", pid);
}

pid_t askForPid() {
	printf("Enter process ID: ");

	int size = 64;
	char input[size];
	fgets(input, size, stdin);

	pid_t pid = 0;
	sscanf(input, "%d\n", &pid);
	return pid;
}

void waitForInterrupt() {
	int size = 64;
	char input[size];
	while (fgets(input, size, stdin)) {
		printf("Waiting for keyboard interrupt...");
	}
}

#endif