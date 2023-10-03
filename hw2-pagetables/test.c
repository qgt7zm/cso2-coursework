#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "mlpt.h"

#include "allocate.h"
#include "translate.h"

void allocate_manual(size_t va) {
    if (ptbr == 0) {
        create_root_table();
    }

    printf("Manually allocating address 0x%lx:\n", va);

    size_t *table = get_root_table();

    for (int level = 1; level <= LEVELS; level++) {
        size_t vpn = get_vpn(va, level);

        printf("- VPN part %d = 0x%lx", level, vpn);
        size_t pte = table[vpn];

        size_t ppn;
        if (is_page_valid(pte)) {
            printf(" (valid)\n");
            
            ppn = get_ppn(pte);
        } else {
            printf(" (invalid)\n");
            
            printf("- Allocated VPN 0x%lx\n", vpn);
            ppn = create_page(vpn);
            
            pte = (ppn << POBITS) | 1;
            table[vpn] = pte;
        }
        
        printf("- Level %d PPN = 0x%lx\n", level, ppn); 

        table = (size_t *) get_page_address(ppn);
    }
    printf("\n");
}

void test_1_layer() {
    // Manually allocate
    allocate_manual(0x000000);
    allocate_manual(0x123456);

    // Pre-allocate
    page_allocate(0x000000);
    page_allocate(0x000008);
    page_allocate(0x123456);

    // Translate allocated
    translate(0x000000); // Page 0x0, 0x[ppn]000
    translate(0x000008); // Page 0x0, 0x[ppn]008
    translate(0x123456); // Page 0xab, 0x[ppn]456

    // Translate unallocated
    translate(0x002004); // Page 0x2, invalid
    translate(0x124789); // Page invalid
}

void test_2_layers() {
    size_t va1 = 0x15723456; // vpns 0x0ab, 0x123
    allocate_manual(va1);
    page_allocate(va1);
    translate(va1);

    size_t va2 = 0x15724987; // vpns 0x0ab, 0x124
    page_allocate(va2);
    translate(va2);

    size_t va3 = 0x15725000; // vpns 0x0ab, 0x125, invalid
    translate(va3);
}

int main(int argc, char *argv[]) {
    initialize();

#if LEVELS == 1
    // size_t vpn = get_vpn(0x123456, 1);
    // printf("- Virtual page number = 0x%lx\n", vpn);
    test_1_layer();
#endif
#if LEVELS == 2
    // size_t va = 0x15723000; // vpns 0x0ab, 0x123
    // for (int i = 1; i <= LEVELS; i++) {
    //     size_t vpn = get_vpn(va, i);
    //     printf("- VPN part %d = 0x%0.3lx\n", i, vpn);
    // }
    test_2_layers();
#endif
#if LEVELS == 4
    size_t va = 0x77b715723000;  // vpns 0x0ef, 0x0dc, 0x0ab, 0x123
    for (int i = 1; i <= LEVELS; i++) {
        size_t vpn = get_vpn(va, i);
        printf("- VPN part %d = 0x%0.3lx\n", i, vpn);
    }
#endif

    printf("Program completed.\n");
    return 0;
}