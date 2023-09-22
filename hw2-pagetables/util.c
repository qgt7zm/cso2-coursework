#include "config.h"

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

size_t ptbr;
int pn_bits = 64 - POBITS;
int table_size = 2 << (POBITS - 4); // 1 * 2^POBITS / 2^3

void initialize() {
    ptbr = 0;
    printf("Set ptbr to 0.\n");
    printf("Page table size is %d entries.\n", table_size);
}

int is_valid(size_t entry) {
    return entry & 1; // Check last bit
}

size_t get_page_number(size_t entry) {
    return entry >> POBITS;
}