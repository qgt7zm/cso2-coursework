#include <stdio.h>
#include "netsim.h"

// Global Variables
char level; // Command-line argument
int msg_count = 0; // Number of messages received
int msg_total = 0; // Number of messages expected
int callback_id; // Callback for rerequest function

// Function Prototypes
char get_checksum(int len, char *data);
void send_get();
void send_ack();
void rerequest(void *args);

// Helper Functions

char get_checksum(int len, char *data) {
    int start = 1;

    char checksum = data[start];
    for (int i = start + 1; i < len; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

void send_get() {
    int len = 5;

    char data[5];
    data[1] = 'G';
    data[2] = 'E';
    data[3] = 'T';
    data[4] = level;

    char checksum = get_checksum(len, data);
    data[0] = checksum; 
    // printf("Data = %s\n", data);
    send(5, data);
    callback_id = setTimeout(*rerequest, 1000, NULL);
}

void send_ack() {
    int len = 5;

    char msg[len];
    msg[1] = 'A';
    msg[2] = 'C';
    msg[3] = 'K';
    msg[4] = (char) msg_count;
    // printf("Reply = %s\n", msg);

    char checksum = get_checksum(len, msg);
    msg[0] = checksum;
    send(5, msg);

    callback_id = setTimeout(*rerequest, 1000, NULL);
}

// Callback functions

void rerequest(void *args) {
    // printf("Rerequesting message\n");
    if (msg_count == 0) {
        send_get();
    } else {
        send_ack();
    }
}

void recvd(size_t len, void* _data) {
    clearTimeout(callback_id);

    char *data = _data;
    // printf("Received message %d = %s\n", msg_count, data);

    // Check if message is corrupted
    // char real_checksum = data[0];
    // char actual_checksum = get_checksum(len, data);
    // printf("Checksums: %d, %d\n", real_checksum, actual_checksum);

    // if (real_checksum != actual_checksum) {
        // Rerequest message
	// rerequest(NULL); 
	// return;
    // }

    // Received correct message

    // char msg_count = data[1];
    if (msg_total == 0) {
        msg_total = (int) data[2];
    }
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);
    msg_count += 1;

    if (msg_count < msg_total) {
        send_ack(); // Request next message
    }
}

// Main function

int main(int argc, char *argv[]) {
    // this code should work without modification
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
        return 1;
    }
    level = argv[1][0];
    // end of working code
    
    send_get();
    waitForAllTimeouts();
}
