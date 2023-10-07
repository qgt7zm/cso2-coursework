#include <stddef.h>
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "allocate.h"
#include "translate.h"

// Globals
size_t ptbr;
static size_t *root_table = 0;

// Allocation Functions

size_t *get_root_table() {
    if (root_table == 0) {
        root_table = (size_t *) ptbr;
    }
    return root_table;
}

void initialize() {
    ptbr = 0;
    printf("Set ptbr = 0\n");
    printf("Levels = %d, Pobits = %d\n", LEVELS, POBITS);
    printf("Page table size = %d (0x%x) bytes\n", TABLE_SIZE_BYTES, TABLE_SIZE_BYTES);
    printf("\n");
}

size_t allocate_table(size_t *table) {
    int err = posix_memalign((void *)(&table), TABLE_SIZE_BYTES, TABLE_SIZE_BYTES);
    if (err != 0) {
        printf("Error while creating root table.\n");
        exit(1);
    }
    memset(table, 0, TABLE_SIZE_BYTES);
    return (size_t) &table[0];
}

void create_root_table() {
    if (ptbr != 0) {
        return;
    }

    printf("Creating root page table:\n");

    ptbr = allocate_table(root_table);
    root_table = (size_t *) ptbr;
    
    printf("- Set ptbr = 0x%lx\n", ptbr);
    printf("\n");    
}

size_t create_page(size_t vpn) {
    size_t *page = 0;
    size_t page_address = allocate_table(page);

    size_t ppn = get_ppn(page_address);
    return ppn;
}

void delete_page(size_t ppn) {
    size_t page_address = get_page_address(ppn);
    size_t *page = (size_t *) page_address;
    memset(page, 0, TABLE_SIZE_BYTES);
    free(page);
}