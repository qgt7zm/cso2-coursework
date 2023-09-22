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

size_t get_page_number(size_t address) {
    return address >> POBITS;
}

size_t get_page_offset(size_t address) {
    return (address << pn_bits) >> pn_bits;
}

int is_valid(size_t entry) {
    return entry & 1;
}

size_t get_page_address(size_t page_num) {
    return page_num << POBITS;
}