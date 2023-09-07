#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Signals
#include <signal.h>

// Shared Memory
#include <sys/mman.h>
#include <fcntl.h>

#include "chat.h"

// Globals
pid_t other_pid;
char inbox_filename[FILENAME_SIZE];
char outbox_filename[FILENAME_SIZE];
char *inbox_data;
char *outbox_data;

// Signal Functions

void displayInbox() {
	printf("New message: ");
	fputs(inbox_data, stdout);
	fflush(stdout);
	inbox_data[0] = '\0'; // Clear inbox
}

void logoutOtherUser() {
	kill(other_pid, SIGINT);
	printf("Logged out other user.\n");
}

void logout() {
	munmap(inbox_data, MAILBOX_SIZE); // Deallocate pointer
	shm_unlink(inbox_filename); // Deallocate shared memory under filename

	munmap(outbox_data, MAILBOX_SIZE);
	shm_unlink(outbox_filename);

	printf("Logged out.\n");
	exit(0);
}

static void handleSignal(int signum) {
	switch(signum) {
		case SIGINT:
			// Exit both users with Ctrl-C
			logoutOtherUser();
		case SIGTERM:
			// Called by other program
			logout();
			break;
		case SIGUSR1:
			displayInbox();
			break;
	}
}

// Main Helper Functions

pid_t loginUser() {
	pid_t pid = getpid();
	printf("Logged in.\n");
	printf("Process ID: %d\n", pid);
	return pid;
}

void setupSignalHandler() {
	// Create signal handler
	struct sigaction action;
	action.sa_handler = handleSignal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	// Register signal actions
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGUSR1, &action, NULL);
}

// Objective: Create a signal-base two-user chat program
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/signals.html
int main() {
	pid_t pid = loginUser();
	setupSignalHandler();

	/* Set-up Inbox */
	getFilename(inbox_filename, pid);
	int inbox_fd = getFileDescriptor(inbox_filename);
	getFileAsString(&inbox_data, inbox_fd);

	/* Set-up Outbox */
	other_pid = getOtherUserPid();
	printf("Other User ID: %d\n", other_pid);
	getFilename(outbox_filename, other_pid);

	int outbox_fd = getFileDescriptor(outbox_filename);
	getFileAsString(&outbox_data, outbox_fd);

	/* Chat With Other User */
	while (1) {
		// Send message
		printf("Send a message: ");
		char *msg = fgets(outbox_data, MAILBOX_SIZE, stdin);
		if (!msg) break; // EOF

		kill(other_pid, SIGUSR1); // Notify other user

		// Wait until received
		while(outbox_data[0]) { usleep(10000); }
		printf("Message received.\n");
	}

	logout();
}
