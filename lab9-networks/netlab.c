#include <stdio.h>
#include "netsim.h"

// Global Variables
char level; // Command-line argument
int msg_count = 0; // Number of messages received
int msg_total = 0; // Number of messages expected
int callback_id; // Callback for rerequest function

// Function Prototypes
void send_get();
void send_ack();
void rerequest(void *args);

// Helper Functions

void send_get() {
    char data[5];
    data[1] = 'G';
    data[2] = 'E';
    data[3] = 'T';
    data[4] = level;
    
    char checksum = data[1] ^ data[2] ^ data[3] ^ data[4];
    data[0] = checksum; 
    // printf("Data = %s\n", data);
    send(5, data);
    callback_id = setTimeout(*rerequest, 1000, NULL);
}

void send_ack() {
    char msg[6];
    msg[1] = 'A';
    msg[2] = 'C';
    msg[3] = 'K';
    msg[4] = (char) msg_count;
    // printf("Reply = %s\n", msg);

    char checksum = msg[1] ^ msg[2] ^ msg[3] ^ msg[4];
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

    // char checksum = data[0];
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
    // FIX ME -- add action if no reply

    waitForAllTimeouts();
}
