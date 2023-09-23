#include "config.h"
#include "mlpt.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void test() {
    if (root_table) {
        root_table[0x123] = 0xabc001;
        root_table[0x124] = 0xdef000;
        root_table[0x125] = 0xabd001;
    }

    printf("\n");
    translate(0x123456); // Page 0x123, 0xabc456
    translate(0x124789); // Page 0x124, invalid
    translate(0x125456); // Page 0x125, 0xabd456
}

int main(int argc, char *argv[]) {
    initialize();

    create_table();
    root_table = &ptbr;

    test();

    printf("Program completed.\n");
    return 0;
}