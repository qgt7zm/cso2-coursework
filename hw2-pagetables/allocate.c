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

// Allocation Functions

void initialize() {
    ptbr = 0;
    printf("Set ptbr to 0.\n");
    printf("Page table size is %d (0x%x) bytes.\n", table_size_bytes, table_size_bytes);
}

void create_table() {
    // posix_memalign((void *)(&ptbr), table_size_bytes, table_size_bytes);
    posix_memalign((void *)(&root_table), table_size_bytes, table_size_bytes);
    ptbr = (size_t) &root_table[0];
    printf("Set ptbr to %lx\n", ptbr);
    // printf("Set ptbr address to %lx\n", &ptbr);
}

void set_ptbr(size_t root_table[]) {
    
}