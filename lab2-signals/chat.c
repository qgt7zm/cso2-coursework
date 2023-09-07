#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>

#include <sys/mman.h>
#include <fcntl.h>
#define _XOPEN_SOURCE 600

#include "chat.h"

const int filename_size = 128;
const int mailbox_size = 4096;

int otherPid;
char inbox_filename[filename_size];
char outbox_filename[filename_size];
char *inbox_data;
char *outbox_data;

void displayInbox() {
	// Display new messages and clear inbox
	printf("Displaying inbox:\n");
}

void stopOtherProgram() {
	kill(otherPid, SIGINT);
	printf("Logged out other user.\n");
}

void cleanup() {
	munmap(inbox_data, mailbox_size);	// Deallocate pointer
	shm_unlink(inbox_filename); // Deallocate shared memory under filename

	munmap(outbox_data, mailbox_size);
	shm_unlink(outbox_filename);

	printf("Logged out.\n");
	exit(0);
}

static void handleSignal(int signum) {
    if (signum == SIGTERM) { // Kill
		cleanup();
	} else if (signum == SIGINT) { // Ctrl-C
		stopOtherProgram();
		cleanup();
	} else if (signum == SIGUSR1) { // User-defined
		displayInbox();
	}
	// Don't care about others
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

int getOtherUserPid() {
	printf("Enter process ID of other user: ");

	int size = 64;
	char input[size];
	fgets(input, size, stdin); // Use fgets to avoid unflushed stdin

	int otherUser = 0;
	sscanf(input, "%d\n", &otherUser); // Scan int from input string
	return otherUser;
}

void getFilename(char filename[], int pid) {
	snprintf(filename, filename_size, "/%d-chat", pid);
}

int getFileDescriptor(char filename[]) {
	// Get inbox file descriptor
	int fd = shm_open(filename, O_CREAT | O_RDWR, 0666);
 	if (fd < 0) { /* something went wrong */ }
	ftruncate(fd, mailbox_size); // Allocate shared memory space
	return fd;
}

void getFileAsString(char file_data[], int fd) {
	// Convert file descriptor to pointer
	file_data = mmap(NULL, mailbox_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	munmap(file_data, mailbox_size);
	if (file_data == (char*) MAP_FAILED) { /* something went wrong */ }
	close(fd); // Deallocate file descriptor, keep pointer
}

// Objective: Create a signal-base two-user chat program
// Source: https://www.cs.virginia.edu/~cr4bd/3130/F2023/labhw/signals.html
int main() {
	pid_t pid = getpid();

	setupSignalHandler();

	printf("Logged in.\n");
	printf("Process ID: %d\n", pid);

	/* Inbox */

	getFilename(inbox_filename, pid);

	int inbox_fd = getFileDescriptor(inbox_filename);
	getFileAsString(inbox_data, inbox_fd);

	/* Outbox */

	otherPid = getOtherUserPid();
	printf("Other user is %d\n", otherPid);
	getFilename(outbox_filename, otherPid);

	int outbox_fd = getFileDescriptor(outbox_filename);
	getFileAsString(inbox_data, outbox_fd);

	int input_size = 256;
	char input_data[input_size];

	while (1) {
		char *line = fgets(input_data, input_size, stdin);
		if (!line) break; // EOF
		printf("input = %s", line);
	}

	// Do some signal stuff
	kill(getpid(), SIGUSR1);

	// Destroy
	cleanup();
}