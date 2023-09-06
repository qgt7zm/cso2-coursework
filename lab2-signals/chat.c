#include <stdio.h>

#include <signal.h>
#include <unistd.h>

#include <sys/mman.h>
#include <fcntl.h>
#define _XOPEN_SOURCE 600

#include "chat.h"

const int filename_size = 128;
const int inbox_size = 4096;

void displayInbox() {
	// Display new messages and clear inbox
	printf("Display inbox\n");
}

void stopOtherProgram() {
	// Kill other program
	printf("Stop other program\n");
}

void cleanup() {
	// Free inbox/outbox
	printf("Cleanup\n");
}

static void handleSignal(int signum) {
    if (signum == SIGTERM) { // Kill
		cleanup();
	} else if (signum == SIGINT) { // Ctrl-C
		cleanup();
		stopOtherProgram();
	} else if (signum == SIGUSR1) { // User-defined
		displayInbox();
	}
	// Don't care about others
}

// Objective: Create a signal-base two-user chat program
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/signals.html
int main() {
	pid_t pid = getpid();

	printf("Hello there!\n");
	printf("Process ID: %d\n", pid);

	// Create signal handler
	struct sigaction action;
	action.sa_handler = handleSignal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;

	// Register signal actions
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGUSR1, &action, NULL);

	// Create shared memory
	char inbox_filename[filename_size];
	snprintf(inbox_filename, filename_size, "/%d-chat", pid);
	printf("Filename: %s\n", inbox_filename);

	// Create inbox file descriptor
	int inbox_fd = shm_open(inbox_filename, O_CREAT | O_RDWR, 0666);
 	if (inbox_fd < 0) { /* something went wrong */ }
	ftruncate(inbox_fd, inbox_size);

	// Attack pointer to inbox
	char *inbox_data = mmap(NULL, inbox_size, PROT_READ | PROT_WRITE, MAP_SHARED, inbox_fd, 0);
	munmap(inbox_data, inbox_size);
	if (inbox_data == (char*) MAP_FAILED) {
	}
	close(inbox_fd); // Deallocate inbox
	munmap(inbox_data, inbox_size);	// Deallocate pointer

	// Do some signal stuff
	kill(getpid(), SIGUSR1);
	kill(getpid(), SIGINT);
	kill(getpid(), SIGTERM);

	// Destroy inbox
	shm_unlink(inbox_filename);
}