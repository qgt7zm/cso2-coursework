#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "globals.h"

#ifndef FUNCS_H
#define FUNCS_H

// Do nothing
// Don't omit this in compilation
__attribute__((noinline)) void emptyFunc() {
	__asm__("");
}

// Get the parent process id
void runSyscall() {
	getppid();
}

// Run the "true" command
void runShellCmd() {
	system("/usr/bin/true || /bin/true");
}

// Signal the current process
void signalCurrentProcess() {
	raise(SIGUSR1);
}

// Signal another process
void signalOtherProcess() {
	waitForSignal = 1;
	kill(otherPid, SIGUSR1);
}

#endif