#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <unistd.h>

// Shared Memory
#include <sys/mman.h>
#include <fcntl.h>

#include "chat.h"

// Chat Helper Methods

int getOtherUserPid() {
	printf("Enter process ID of other user: ");

	int size = 64;
	char input[size];
	fgets(input, size, stdin); // Use fgets() to avoid unflushed stdin

	int other_user = 0;
	sscanf(input, "%d\n", &other_user); // Scan int from input string
	return other_user;
}

void getFilename(mailbox *mb) {
	snprintf(mb->filename, FILENAME_SIZE, "/%d-chat", mb->pid);
}

int getFileDescriptor(char filename[]) {
	int fd = shm_open(filename, O_CREAT | O_RDWR, 0666);
 	if (fd < 0) { /* something went wrong */ }
	ftruncate(fd, MAILBOX_SIZE); // Allocate shared memory space
	return fd;
}

void getFileAsString(char **file_contents, int fd) {
	// Attach pointer to file descriptor
	*file_contents = mmap(NULL, MAILBOX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (*file_contents == (char*) MAP_FAILED) { /* something went wrong */ }
	close(fd); // Deallocate file descriptor, keep pointer
}

void getContents(mailbox *mb) {
	getFilename(mb);
	int fd = getFileDescriptor(mb->filename);
	getFileAsString(&(mb->contents), fd);
}

void freeMailbox(mailbox *mb) {
	munmap(mb->contents, MAILBOX_SIZE); // Deallocate pointer
	shm_unlink(mb->filename); // Deallocate shared memory under filename
}