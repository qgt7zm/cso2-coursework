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

// Signal Functions

void displayMailbox(mailbox mb) {
	printf("New message: ");
	fputs(mb.contents, stdout);
	fflush(stdout);
	mb.contents[0] = '\0'; // Clear mailbox
}

void logoutOtherUser() {
	kill(outbox.pid, SIGINT);
	printf("Logged out other user.\n");
}

void logout() {
	freeMailbox(&inbox);
	freeMailbox(&outbox);

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
			displayMailbox(inbox);
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
	getContents(&inbox);

	/* Set-up Outbox */
	outbox.pid = getOtherUserPid();
	printf("Other User ID: %d\n", outbox.pid);
	getContents(&outbox);

	/* Chat With Other User */
	while (1) {
		// Send message
		printf("Send a message: ");
		char *msg = fgets(outbox.contents, MAILBOX_SIZE, stdin);
		if (!msg) break; // EOF

		kill(outbox.pid, SIGUSR1); // Notify other user

		// Wait until received
		while(outbox.contents[0]) { usleep(10000); }
		printf("Message received.\n");
	}

	logout();
}
