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

// Allocation Functions

void initialize() {
    ptbr = 0;
    printf("Set ptbr = 0.\n");
    printf("Levels = %d, Pobits = %d\n", LEVELS, POBITS);
    printf("Page table size = %d (0x%x) bytes\n", table_size_bytes, table_size_bytes);
    printf("\n");
}

size_t *create_root_table() {
    printf("Creating root page table:\n");

    size_t* root_table = 0;    
    posix_memalign((void *)(&root_table), table_size_bytes, table_size_bytes);
    ptbr = (size_t) &root_table[0];
    
    printf("- Set ptbr = 0x%lx\n", ptbr);
    printf("\n");

    root_table = (size_t *) ptbr;
    return root_table;
}

size_t create_page(size_t vpn) {
    size_t *table = 0;    
    posix_memalign((void *)(&table), table_size_bytes, table_size_bytes);
    size_t page_address = (size_t) &table[0];

    size_t ppn = get_page_number(page_address);
    return ppn;
}