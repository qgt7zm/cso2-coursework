#ifndef CHAT_H
#define CHAT_H

// Constants
#define FILENAME_SIZE 128
#define MAILBOX_SIZE 4096

// Chat Helper Methods

int getOtherUserPid();
void getFilename(char filename[], int pid);
int getFileDescriptor(char filename[]);
void getFileAsString(char **file_data, int fd);

#endif