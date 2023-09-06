# include <signal.h>
# include <stdio.h>
# include <unistd.h>

void printPid() {
	pid_t processPid = getpid();
	printf("Process ID: %d\n", processPid);
}

void displayInbox() {
	printf("Display inbox\n");
}

void stopOtherProgram() {
	printf("Stop other program\n");
}

void cleanup() {
	printf("Cleanup\n");
}

static void handleSignal(int signum) {
    if (signum == SIGTERM) {
		// Kill: Cleanup (clear inbox/outbox)
		cleanup();
	} else if (signum == SIGINT) {
		// Ctrl-C: Cleanup and kill other program
		cleanup();
		stopOtherProgram();
	} else if (signum == SIGUSR1) {
		// User-defined: display inbox and reset
		displayInbox();
	}
	// Don't care about others
}

int main() {
	printf("Hello there!\n");
	printPid();

	// Create signal handler
	struct sigaction action;
	action.sa_handler = handleSignal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	// Register signal actions
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGUSR1, &action, NULL);

	kill(getpid(), SIGUSR1);
	kill(getpid(), SIGINT);
	kill(getpid(), SIGTERM);
}