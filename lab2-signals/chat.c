#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Signals
#include <signal.h>

// Shared Memory
#include <sys/mman.h>
#include <fcntl.h>

// Constants
const int filename_size = 128;
const int mailbox_size = 4096;

// Globals
pid_t other_pid;
char inbox_filename[filename_size];
char outbox_filename[filename_size];
char *inbox_data;
char *outbox_data;

// Signal Functions

void displayInbox() {
	// Display new messages and clear inbox
	printf("Displaying inbox:\n");
	fputs(inbox_data, stdout);
	fflush(stdout);
	inbox_data[0] = '\0';
}

void stopOtherProgram() {
	kill(other_pid, SIGINT);
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
    if (signum == SIGINT) {
		// Exit both programs on Ctrl-C
		stopOtherProgram();
		cleanup();
	} else if (signum == SIGTERM) {
		// Called by other program
		cleanup();
	} else if (signum == SIGUSR1) {
		displayInbox();
	}
}

// Main Helper Functions

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
	// Convert file descriptor to pointer
	inbox_data = mmap(NULL, mailbox_size, PROT_READ | PROT_WRITE, MAP_SHARED, inbox_fd, 0);
	if (inbox_data == (char*) MAP_FAILED) { /* something went wrong */ }
	close(inbox_fd); // Deallocate file descriptor, keep pointer

	/* Outbox */

	other_pid = getOtherUserPid();
	printf("Other User ID: %d\n", other_pid);
	getFilename(outbox_filename, other_pid);

	int outbox_fd = getFileDescriptor(outbox_filename);
	outbox_data = mmap(NULL, mailbox_size, PROT_READ | PROT_WRITE, MAP_SHARED, outbox_fd, 0);
	if (outbox_data == (char*) MAP_FAILED) {}
	close(outbox_fd);

	int input_size = 512;
	char input_data[input_size];

	printf("inbox = '%s'\n", inbox_data);
	printf("outbox = '%s'\n", outbox_data);

	while (1) {
		// Send message
		printf("Type a message to send: ");
		char *msg = fgets(outbox_data, mailbox_size, stdin);
		if (!msg) break; // EOF
		printf("Sent message = %s", msg);

		// Wait until received
		while(outbox_data[0]) { usleep(10000); }
		printf("Message received.\n");

		kill(getpid(), SIGUSR1); // Display inbox
	}

	// Destroy
	cleanup();
}