#include <stdio.h>
#include "netsim.h"

char level; // Command-line argument
int msg_count = 0; // Number of messages received

void foo(void* _data) {
    printf("Your mom\n");
}

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
}

void recvd(size_t len, void* _data) {
    char *data = _data;
    // printf("Received message %d = %s\n", msg_count, data);
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);

    msg_count += 1;
    send_ack();
}

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
    // setTimeout(*foo, 1000, NULL);

    waitForAllTimeouts();
}
