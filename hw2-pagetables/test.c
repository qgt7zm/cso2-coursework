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

    size_t pte = (ppn << POBITS) | 1;
    get_root_table()[vpn] = pte;
}

void allocate_manual_multi_level(size_t va) {
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
            
            printf("- Allocated page 0x%lx\n", vpn);
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
    allocate_manual(0x0, 0x0);
    // allocate_manual(0x123, 0xabc);

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

void test_2_layers() {
    size_t va1 = 0x2af0000123456; // vpns 0xabc, 0x123

    allocate_manual_multi_level(va1);
    // size_t va2 = 0x2af0000124987; // vpns 0xabc, 0x124
    page_allocate(va1);
    // page_allocate(va2);
    translate(va1);
    // translate(va2);

    // size_t va3 = 0x2af0000125000; // vpns 0xabc, 0x125
    // translate(va3);
}

int main(int argc, char *argv[]) {
    initialize();

#if LEVELS == 1
    // size_t vpn = get_vpn(0x123456, 1);
    // printf("- Virtual page number = 0x%lx\n", vpn);
    test_1_layer();
#endif
#if LEVELS == 2
    // size_t va = 0x2af0000123456; // vpns 0xabc, 0x123
    // for (int i = 1; i <= LEVELS; i++) {
    //     size_t vpn = get_vpn(va, i);
    //     printf("- VPN part %d = 0x%lx\n", i, vpn);
    // }
    test_2_layers();
#endif
#if LEVELS == 4
    size_t va = 0x55e3fb4246789456;  // vpns 0xabc 0xfed 0x789, 0x123
    for (int i = 1; i <= LEVELS; i++) {
        size_t vpn = get_vpn(va, i);
        printf("- VPN part %d = 0x%lx\n", i, vpn);
    }
#endif

    printf("Program completed.\n");
    return 0;
}