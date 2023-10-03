#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "allocate.h"
#include "translate.h"

// Constants
const int table_size_bytes = 2 << (POBITS - 1); // 1 * 2^POBITS
const int table_size_entries = 2 << (POBITS - 4); // 1 * 2^POBITS / 8

// Globals
size_t ptbr;
static size_t *root_table = 0;

size_t *get_root_table() {
    if (root_table == 0) {
        root_table = (size_t *) ptbr;
    }
    return root_table;
}

// Allocation Functions

void initialize() {
    ptbr = 0;
    printf("Set ptbr = 0\n");
    printf("Levels = %d, Pobits = %d\n", LEVELS, POBITS);
    printf("VPN bits = %d per part\n", vpn_bits);
    printf("Page table size = %d (0x%x) bytes\n", table_size_bytes, table_size_bytes);
    printf("\n");
}

void create_root_table() {
    if (ptbr != 0) {
        return;
    }

    printf("Creating root page table:\n");

    int err = posix_memalign((void *)(&root_table), table_size_bytes, table_size_bytes);
    if (err != 0) {
        printf("Error while creating root table.\n");
        exit(1);
    }
    ptbr = (size_t) &root_table[0];
    root_table = (size_t *) ptbr;
    
    printf("- Set ptbr = 0x%lx\n", ptbr);
    printf("\n");    
}

size_t create_page(size_t vpn) {
    size_t *page = 0;    
    int err = posix_memalign((void *)(&page), table_size_bytes, table_size_bytes);
    if (err != 0) {
        printf("Error while allocating page.\n");
        exit(1);
    }
    size_t page_address = (size_t) &page[0];

    size_t ppn = get_ppn(page_address);
    return ppn;
}