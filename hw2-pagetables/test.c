#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "mlpt.h"

#include "allocate.h"
#include "translate.h"

void test() {
    // Pre-allocate
    page_allocate(0x000000);
    page_allocate(0x000008);
    page_allocate(0x123456);

    // Translate allocated
    translate(0x000000); // Page 0x0, 0x[ppn]000
    translate(0x000008); // Page 0x0, 0x[ppn]008
    translate(0x123456); // Page 0x123, 0x[ppn]456

    // Translate unallocated
    translate(0x002004); // Page 0x2, invalid
    translate(0x124789); // Page 0x124, invalid
}

int main(int argc, char *argv[]) {
    initialize();
    test();

    printf("Program completed.\n");
    return 0;
}