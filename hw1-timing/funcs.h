#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef FUNCS_H
#define FUNCS_H

// Do nothing
void emptyFunc() {
}

// Get the parent process id
void runSyscall() {
	getppid();
}

// Run the "true" command
void runShellCmd() {
	system("/bin/true || /usr/bin/true");
}

// Signal the current process
void signalCurrentProcess() {
	kill(getpid(), SIGUSR1);
}

// Signal another process
void signalOtherProcess(pid_t pid) {
	kill(pid, SIGUSR1);
}

#endif