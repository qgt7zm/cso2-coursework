#include "config.h"
#include "mlpt.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void test_translate() {
    root_table = (size_t *) ptbr;

    if (root_table) {
        root_table[0x0] = 0x001001;
        root_table[0x1] = 0x000001;
        root_table[0x2] = 0x011001;

        root_table[0x123] = 0xabc001;
        root_table[0x124] = 0xdef000;
    }

    printf("\n");

    translate(0x000000); // Page 0x0, 0x000000
    translate(0x000008); // Page 0x0, 0x000008
    translate(0x002004); // Page 0x2, 0x000004

    translate(0x123456); // Page 0x123, 0xabc456
    translate(0x124789); // Page 0x124, invalid
}

int main(int argc, char *argv[]) {
    initialize();

    create_table();
    test_translate();

    printf("Program completed.\n");
    return 0;
}