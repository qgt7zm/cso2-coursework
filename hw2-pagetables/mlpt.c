#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "mlpt.h"

#include "allocate.h"
#include "translate.h"

// Constants
const size_t invalid_address = ~0;

// Functions

size_t translate(size_t va) {
    printf("Translating virtual address 0x%lx:\n", va);

    if (!ptbr) {
        printf("- Error: Root page table not allocated yet. \n");
        printf("- Physical address = 0x%lx\n", invalid_address);
        printf("\n");
        return invalid_address;
    } 

    size_t vpn = get_vpn(va, 1);
    printf("- Virtual page number = 0x%lx", vpn);

    size_t pte = get_root_table()[vpn];
    // printf("- Page entry = 0x%lx\n", pte);

    if (is_page_valid(pte)) {
        printf(" (valid)\n");
    } else {
        printf(" (invalid)\n");
        printf("- Physical address = 0x%lx\n", invalid_address);
        printf("\n");
        return invalid_address;
    }

    size_t ppn = get_ppn(pte);
    printf("- Physical page number = 0x%lx\n", ppn);

    size_t phys_addr = get_page_address(ppn) + get_page_offset(va);
    printf("- Physical address = 0x%lx\n", phys_addr);
    
    printf("\n");
    return phys_addr;
}

void page_allocate(size_t va) {
    // Check if base table is allocated
    if (ptbr == 0) {
        create_root_table();
    }

    printf("Allocating address 0x%lx:\n", va);

    // Check if virtual page is valid
    size_t vpn = get_vpn(va, 1);
    printf("- Virtual page number = 0x%lx\n", vpn);

    size_t pte = get_root_table()[vpn];
    int valid = is_page_valid(pte);
    if (valid) {
        printf("- Page is valid\n");
    } else {
        printf("- Page is invalid\n");

        // Allocate the page
        size_t ppn = create_page(vpn);

        // Set the page as valid
        size_t pte = (ppn << POBITS) | 1;
        get_root_table()[vpn] = pte;
        printf("- Allocated page 0x%lx\n", vpn); 

        // size_t ppn = get_ppn(pte);
        printf("- Physical page number = 0x%lx\n", ppn); 
    }

    printf("\n");
    return;
}