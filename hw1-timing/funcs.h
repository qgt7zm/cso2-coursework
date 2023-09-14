#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

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
	system("/usr/bin/true");
}

// Signal the current process
void signalCurrentProcess() {
	raise(SIGUSR1);
}

// Signal another process
void signalOtherProcess(pid_t pid) {
	kill(pid, SIGUSR1);
	// TODO 2.6 make sure signal is received
}

#endif