#include <stdio.h>
#include "netsim.h"

int msg_count = 0;

void foo(void* _data) {
    printf("Your mom\n");
}

void recvd(size_t len, void* _data) {
    // FIX ME -- add proper handling of messages
    char *data = _data;
    // printf("Received message %d = %s\n", msg_count, data);
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);

    msg_count += 1;

    char msg[6];
    msg[1] = 'A';
    msg[2] = 'C';
    msg[3] = 'K';
    msg[4] = msg_count;
    // printf("Reply = %s\n", msg);
    
    char checksum = msg[1] ^ msg[2] ^ msg[3] ^ msg[4];
    msg[0] = checksum;
    send(5, msg);
    // setTimeout(*foo, 1000, NULL);
}

int main(int argc, char *argv[]) {
    // this code should work without modification
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
        return 1;
    }
    char data[5];
    data[1] = 'G'; data[2] = 'E'; data[3] = 'T'; data[4] = argv[1][0];
    // end of working code
    
    char checksum = data[1] ^ data[2] ^ data[3] ^ data[4];
    data[0] = checksum; 
    // printf("Data = %s\n", data);
    send(5, data);
    // FIX ME -- add action if no reply
    // setTimeout(*foo, 1000, NULL);

    waitForAllTimeouts();
}
