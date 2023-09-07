#ifndef CHAT_H
#define CHAT_H

#include <signal.h>

// Constants
#define FILENAME_SIZE 128
#define MAILBOX_SIZE 4096

// Structs
typedef struct {
	pid_t pid;
	char filename[FILENAME_SIZE];
	char *contents;
} mailbox;

// Chat Helper Methods

int getOtherUserPid();
void getFilename(char filename[], int pid);
int getFileDescriptor(char filename[]);
void getFileAsString(char **file_data, int fd);

#endif