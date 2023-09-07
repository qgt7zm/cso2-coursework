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
mailbox inbox;
mailbox outbox;

// Mailbox Functions

void displayInbox() {
	printf("New message: ");
	fputs(inbox.contents, stdout);
	fflush(stdout);
	inbox.contents[0] = '\0'; // Clear inbox
}

void logoutOtherUser() {
	kill(outbox.pid, SIGINT);
	printf("Logged out other user.\n");
}

void logout() {
	munmap(inbox.contents, MAILBOX_SIZE); // Deallocate pointer
	shm_unlink(inbox.filename); // Deallocate shared memory under filename

	munmap(outbox.contents, MAILBOX_SIZE);
	shm_unlink(outbox.filename);

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
	inbox.pid = loginUser();
	setupSignalHandler();

	/* Set-up Inbox */
	getFilename(inbox.filename, inbox.pid);
	int inbox_fd = getFileDescriptor(inbox.filename);
	getFileAsString(&(inbox.contents), inbox_fd);

	/* Set-up Outbox */
	outbox.pid = getOtherUserPid();
	printf("Other User ID: %d\n", outbox.pid);
	
	getFilename(outbox.filename, outbox.pid);
	int outbox_fd = getFileDescriptor(outbox.filename);
	getFileAsString(&(outbox.contents), inbox_fd);

	/* Chat With Other User */
	while (1) {
		// Send message
		printf("Send a message: ");
		printf("contents = %s", outbox.contents);
		char *msg = fgets(outbox.contents, MAILBOX_SIZE, stdin);
		if (!msg) break; // EOF

		kill(outbox.pid, SIGUSR1); // Notify other user

		// Wait until received
		while(outbox.contents[0]) { usleep(10000); }
		printf("Message received.\n");
	}

	logout();
}
