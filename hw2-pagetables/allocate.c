#include <stddef.h>
#define _XOPEN_SOURCE 700
#include "config.h"

#include "allocate.h"
#include "translate.h"

#include <stdio.h>
#include <stdlib.h>

// Constants
const int table_size_bytes = 2 << (POBITS - 1); // 1 * 2^POBITS
const int table_size_entries = 2 << (POBITS - 4); // 1 * 2^POBITS / 8

// Globals
size_t ptbr;
size_t* root_table = 0;
int tables_allocated = 0; // Track how many tables have been allocated

// Allocation Functions

void initialize() {
    ptbr = 0;
    printf("Set ptbr = 0.\n");
    printf("Levels = %d, Pobits = %d\n", LEVELS, POBITS);
    printf("Page table size = %d (0x%x) bytes\n", table_size_bytes, table_size_bytes);
    printf("\n");
}

void create_root_table() {
    printf("Creating root page table:\n");
    posix_memalign((void *)(&root_table), table_size_bytes, table_size_bytes);
    ptbr = (size_t) &root_table[0];
    root_table = (size_t *) ptbr;
    printf("- Set ptbr = 0x%lx\n", ptbr);
    printf("\n");
}