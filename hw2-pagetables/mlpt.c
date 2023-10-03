#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "mlpt.h"

#include "allocate.h"
#include "translate.h"

// Constants
const size_t invalid_address = ~0;

// Helper Functions
size_t print_invalid_address() {
    printf("- Physical address = 0x%lx\n", invalid_address);
    printf("\n");
    return invalid_address;
}

// Functions

size_t translate(size_t va) {
    printf("Translating virtual address 0x%lx:\n", va);

    // Check if base table is allocated
    if (ptbr == 0) {
        printf("- Error: Root page table not allocated yet.\n");
        // return 0x2; // reaches here on translate w/ allocate
        return print_invalid_address();
    }

    size_t phys_addr;
    size_t *table = get_root_table();

    for (int level = 1; level <= LEVELS; level++) {
        // printf("- Level %d table = 0x%p\n", level, table);
        
        // Check if table is present for manual allocation
        if (table == 0) {
            printf("- Error: Level %d table not allocated yet.\n", level);
            // return 0x3; // doesn't reach here
            return print_invalid_address();
        }
        
        size_t vpn = get_vpn(va, level);
        printf("- VPN part %d = 0x%lx", level, vpn);

        size_t pte = table[vpn];
        // printf("- Page entry = 0x%lx", pte);

        if (is_page_valid(pte)) {
            printf(" (valid)\n");
            // if (level < LEVELS) {
            //     return 0x50 + level; // reaches here on translate w/ manual allocate
            // }
        } else {
            printf(" (invalid)\n");
            // return 0x4; // reaches here on translate w/ manual allocate
            return print_invalid_address();
        }

        size_t ppn = get_ppn(pte);
        printf("- PPN level %d = 0x%lx\n", level, ppn);

        phys_addr = get_page_address(ppn);
        if (level == LEVELS) {
            // Add page offset to final page
            phys_addr += get_page_offset(va);
        }
        // printf("- Physical address = 0x%lx\n", phys_addr);
        
        table = (size_t *) phys_addr; // Address of next table
    }

    printf("- Physical address = 0x%lx\n", phys_addr);
    printf("\n");

    
    return phys_addr;
}

void page_allocate(size_t va) {
    // Make sure base table is allocated
    if (ptbr == 0) {
        create_root_table();
    }

    printf("Allocating address 0x%lx:\n", va);

    size_t *table = get_root_table();

    for (int level = 1; level <= LEVELS; level++) {
        // printf("- Level %d table = 0x%p\n", level, table);

        // Check if table is present for manual allocation
        if (table == 0) {
            printf("- Error: Level %d table not allocated yet.\n", level);
            return;
        }

        size_t vpn = get_vpn(va, level);
        printf("- VPN part %d = 0x%lx", level, vpn);

        // Check if virtual page is valid
        size_t pte = table[vpn];
        size_t ppn;
        if (is_page_valid(pte)) {
            printf(" (valid)\n");

            ppn = get_ppn(pte);
        } else {
            printf(" (invalid)\n");

            // Allocate the page
            ppn = create_page(vpn);

            // Set the page as valid
            size_t pte = (ppn << POBITS) | 1;
            table[vpn] = pte;
            printf("- Allocated page 0x%lx\n", vpn); 

            // size_t ppn = get_ppn(pte);
            printf("- Level %d PPN = 0x%lx\n", level, ppn); 
        }

        // Address of next table
        table = (size_t *) get_page_address(ppn);
    }

    printf("\n");
    return;
}