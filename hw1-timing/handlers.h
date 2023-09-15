#include <stdio.h>
#include <unistd.h>

#include <signal.h>

#include "timer.h"

#ifndef HANDLERS_H
#define HANDLERS_H

// Globals
int signalsReceived = 0;
static sigset_t sig_set;

// Signal Handler Functions
void handleSignal(int signum) {
	if (signum == SIGUSR1) {
		stopTimer(); // Stop the timer after the handler starts
		signalsReceived += 1;
		printf("Process %d received a signal!\n", getpid());
	}
}

void setSignalHandler() {
	struct sigaction action;
	action.sa_handler = handleSignal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	sigaction(SIGUSR1, &action, NULL);
}

// Signal Blocker Functions
void setSignalBlocker() {
	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGUSR1);
}

void blockSignals() {
	sigprocmask(SIG_BLOCK, &sig_set, NULL);
}

void waitForSignalRecieved() {
	int sig = 0;
	do {
		printf("Waiting...\n");
		sigwait(&sig_set, &sig);
		printf("Got sig = %d\n", sig);
	}
	while (sig != SIGUSR1);
	// Other program received signal
	printf("Other program received signal.\n");
}

void unblockSignals() {
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL);
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

	int pid = 0;
	sscanf(input, "%d\n", &pid);
	return pid;
}

void waitForInterrupt() {
	int size = 64;
	char input[size];
	do {
		printf("Waiting for keyboard interrupt... ");
	} while (fgets(input, size, stdin));
}

#endif