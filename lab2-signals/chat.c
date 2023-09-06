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
	int otherUser;
	printf("Enter process ID of other user: ");
	scanf("%d", &otherUser);
	return otherUser;
}

void getFilename(char filename[], int pid) {
	snprintf(filename, filename_size, "/%d-chat", pid);
}

int getFileDescriptor(char filename[]) {
	// Get inbox file descriptor
	int fd = shm_open(filename, O_CREAT | O_RDWR, 0666);
 	if (fd < 0) { /* something went wrong */ }
	ftruncate(fd, inbox_size); // Allocate shared memory space
	return fd;
}

void getFileAsString(char file_data[], int fd) {
	// Convert file descriptor to pointer
	file_data = mmap(NULL, inbox_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	munmap(file_data, inbox_size);
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

	char inbox_filename[filename_size];
	getFilename(inbox_filename, pid);
	printf("Inbox filename: %s\n", inbox_filename);

	int inbox_fd = getFileDescriptor(inbox_filename);
	char *inbox_data;
	getFileAsString(inbox_data, inbox_fd);

	/* Outbox */

	int otherPid = getOtherUserPid();
	printf("Other user is %d\n", otherPid);

	char outbox_filename[filename_size];
	getFilename(outbox_filename, otherPid);
	printf("Outbox filename: %s\n", outbox_filename);

	int outbox_fd = getFileDescriptor(outbox_filename);
	char *outbox_data;
	getFileAsString(inbox_data, outbox_fd);

	// Do some signal stuff
	kill(getpid(), SIGUSR1);
	kill(getpid(), SIGINT);
	kill(getpid(), SIGTERM);

	// Destroy inbox
	munmap(inbox_data, inbox_size);	// Deallocate pointer
	shm_unlink(inbox_filename); // Deallocate shared memory under filename

	munmap(outbox_data, inbox_size);
	shm_unlink(outbox_filename);
}