#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "mlpt.h"

#include "allocate.h"
#include "translate.h"

void allocate_manual(size_t vpn, size_t ppn) {
    if (ptbr == 0) {
        create_root_table();
    }

    printf("Manually allocating page:\n");
    printf("- Allocated page 0x%lx\n", vpn); 
    printf("- Physical page number = 0x%lx\n", ppn); 
    printf("\n");

    size_t pte = ppn << POBITS | 1;
    get_root_table()[vpn] = pte;
}

void test_1_layer() {
    // Manually allocate
    allocate_manual(0x0, 0x0);
    // allocate_manual(0x123, 0xabc);

    // Pre-allocate
    page_allocate(0x000000);
    // page_allocate(0x000008);
    page_allocate(0x123456);

    // Translate allocated
    // translate(0x000000); // Page 0x0, 0x[ppn]000
    // translate(0x000008); // Page 0x0, 0x[ppn]008
    translate(0x123456); // Page 0x123, 0x[ppn]456

    // Translate unallocated
    translate(0x002004); // Page 0x2, invalid
    // translate(0x124789); // Page 0x124, invalid
}

int main(int argc, char *argv[]) {
    initialize();
    test_1_layer();

    printf("Program completed.\n");
    return 0;
}